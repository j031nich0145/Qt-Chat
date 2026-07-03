#include "MainWindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Supa LLM Client");
    resize(900, 650);

    m_groqClient    = new GroqClient(this);
    m_ollamaManager = new OllamaManager(this);
    m_themeManager  = new ThemeManager(this);

    setupMenuBar();

    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

    layout->addWidget(setupBackendBar());

    m_chatView = new QWebEngineView(this);

    auto *inputRow = new QHBoxLayout();
    m_inputField   = new QLineEdit(this);
    m_inputField->setPlaceholderText("Type a message...");
    m_sendButton   = new QPushButton("Send", this);

    inputRow->addWidget(m_inputField);
    inputRow->addWidget(m_sendButton);

    layout->addWidget(m_chatView);
    layout->addLayout(inputRow);

    setCentralWidget(central);

    loadSettings();

    connect(m_chatView, &QWebEngineView::loadFinished, this, &MainWindow::onPageLoaded);
    m_chatView->load(QUrl("qrc:/web/web/chat.html"));

    connect(m_sendButton, &QPushButton::clicked,   this, &MainWindow::onSendClicked);
    connect(m_inputField, &QLineEdit::returnPressed, this, &MainWindow::onSendClicked);

    connect(m_groqClient, &GroqClient::tokenReceived,    this, &MainWindow::onTokenReceived);
    connect(m_groqClient, &GroqClient::responseFinished, this, &MainWindow::onResponseFinished);
    connect(m_groqClient, &GroqClient::errorOccurred,    this, &MainWindow::onErrorOccurred);

    connect(m_ollamaManager, &OllamaManager::tokenReceived,    this, &MainWindow::onTokenReceived);
    connect(m_ollamaManager, &OllamaManager::responseFinished, this, &MainWindow::onResponseFinished);
    connect(m_ollamaManager, &OllamaManager::errorOccurred,    this, &MainWindow::onErrorOccurred);
    connect(m_ollamaManager, &OllamaManager::installedModelsChanged,
            this, &MainWindow::onOllamaModelsChanged);

    connect(m_themeManager, &ThemeManager::customThemesChanged,
            this, &MainWindow::rebuildThemeMenu);
}

QWidget *MainWindow::setupBackendBar() {
    auto *bar = new QWidget(this);
    auto *row = new QHBoxLayout(bar);
    row->setContentsMargins(0, 0, 0, 0);

    row->addWidget(new QLabel("Backend:", this));
    m_backendCombo = new QComboBox(this);
    m_backendCombo->addItem("Groq (cloud)", static_cast<int>(Backend::Groq));
    m_backendCombo->addItem("Ollama (local)", static_cast<int>(Backend::Ollama));
    row->addWidget(m_backendCombo);

    row->addWidget(new QLabel("Model:", this));
    m_modelCombo = new QComboBox(this);
    row->addWidget(m_modelCombo, 1);

    connect(m_backendCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onBackendChanged);

    onBackendChanged(0);

    return bar;
}

void MainWindow::setupMenuBar() {
    QMenu *settingsMenu = menuBar()->addMenu("Settings");

    QAction *groqAction   = new QAction("Configure Groq API", this);
    QAction *ollamaAction = new QAction("Configure Ollama", this);

    settingsMenu->addAction(groqAction);
    settingsMenu->addSeparator();
    settingsMenu->addAction(ollamaAction);

    connect(groqAction,   &QAction::triggered, this, &MainWindow::onConfigureGroq);
    connect(ollamaAction, &QAction::triggered, this, &MainWindow::onConfigureOllama);

    m_themeMenu = menuBar()->addMenu("Theme");
    rebuildThemeMenu();
}

void MainWindow::rebuildThemeMenu() {
    m_themeMenu->clear();

    for (const Theme &t : m_themeManager->presets()) {
        QAction *action = new QAction(t.name, this);
        connect(action, &QAction::triggered, this, [this, name = t.name]() {
            onThemeSelected(name);
        });
        m_themeMenu->addAction(action);
    }

    QList<Theme> customs = m_themeManager->customThemes();
    if (!customs.isEmpty()) {
        m_themeMenu->addSeparator();
        for (const Theme &t : customs) {
            QAction *action = new QAction(t.name, this);
            connect(action, &QAction::triggered, this, [this, name = t.name]() {
                onThemeSelected(name);
            });
            m_themeMenu->addAction(action);
        }
    }

    m_themeMenu->addSeparator();
    QAction *customAction = new QAction("Custom Theme...", this);
    connect(customAction, &QAction::triggered, this, &MainWindow::onCustomTheme);
    m_themeMenu->addAction(customAction);

    QAction *manageAction = new QAction("Manage Custom Themes...", this);
    connect(manageAction, &QAction::triggered, this, &MainWindow::onManageThemes);
    m_themeMenu->addAction(manageAction);
}

void MainWindow::onBackendChanged(int index) {
    m_backend = static_cast<Backend>(m_backendCombo->itemData(index).toInt());
    m_modelCombo->clear();

    if (m_backend == Backend::Groq) {
        m_modelCombo->addItem("openai/gpt-oss-20b");
        m_modelCombo->addItem("openai/gpt-oss-120b");
    } else {
        // Ollama: show installed models
        m_ollamaManager->refreshInstalledModels();
    }
}

void MainWindow::onOllamaModelsChanged(const QStringList &models) {
    if (m_backend != Backend::Ollama) return;

    m_modelCombo->clear();
    if (models.isEmpty()) {
        m_modelCombo->addItem("(no models — see Configure Ollama)");
    } else {
        m_modelCombo->addItems(models);
    }
}

void MainWindow::loadSettings() {
    QSettings settings("SupaLLM", "SupaLLMClient");
    m_groqApiKey = settings.value("groq/apiKey", "").toString();
    m_groqClient->setApiKey(m_groqApiKey);
    m_themeManager->loadSaved();
}

void MainWindow::onPageLoaded(bool ok) {
    if (!ok) return;
    m_pageReady = true;
    pushTheme();
    updateMessages();
}

void MainWindow::pushTheme() {
    if (!m_pageReady) return;
    Theme t = m_themeManager->currentTheme();

    QString js = QString("setTheme('%1','%2','%3','%4','%5',%6);")
        .arg(t.background)
        .arg(t.text)
        .arg(t.surface)
        .arg(t.accent)
        .arg(t.fontFamily)
        .arg(t.fontSize);

    m_chatView->page()->runJavaScript(js);
}

void MainWindow::updateMessages() {
    if (!m_pageReady) return;

    QJsonArray arr;
    for (const Message &msg : m_messages) {
        QJsonObject obj;
        obj["role"]    = (msg.role == Role::User) ? "user" : "assistant";
        obj["content"] = msg.content;
        arr.append(obj);
    }

    QString json = QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact));
    QString encoded = QString::fromUtf8(json.toUtf8().toBase64());

    QString js = QString("render(decodeURIComponent(escape(atob('%1'))));").arg(encoded);
    m_chatView->page()->runJavaScript(js);
}

QJsonArray MainWindow::buildMessageHistory() {
    QJsonArray arr;
    for (const Message &msg : m_messages) {
        if (msg.content.isEmpty()) continue;  // skip the pending assistant slot
        QJsonObject obj;
        obj["role"]    = (msg.role == Role::User) ? "user" : "assistant";
        obj["content"] = msg.content;
        arr.append(obj);
    }
    return arr;
}

void MainWindow::onSendClicked() {
    QString text = m_inputField->text().trimmed();
    if (text.isEmpty()) return;

    if (m_backend == Backend::Groq && m_groqApiKey.isEmpty()) {
        QMessageBox::warning(this, "No API Key", "Please configure your Groq API key first.");
        return;
    }

    QString model = m_modelCombo->currentText();
    if (m_backend == Backend::Ollama &&
        (model.isEmpty() || model.startsWith("("))) {
        QMessageBox::warning(this, "No Model",
            "No Ollama model selected. Open Settings → Configure Ollama to download one.");
        return;
    }

    m_messages.append({ Role::User, text, 0 });

    // Build history BEFORE appending the empty assistant slot
    QJsonArray history = buildMessageHistory();

    m_messages.append({ Role::Assistant, "", 0 });
    m_currentAssistantMsg.clear();

    m_inputField->clear();
    m_sendButton->setEnabled(false);
    updateMessages();

    if (m_backend == Backend::Groq) {
        m_groqClient->sendMessage(text);
    } else {
        m_ollamaManager->sendMessage(model, history);
    }
}

void MainWindow::onTokenReceived(const QString &token) {
    m_currentAssistantMsg += token;
    if (!m_messages.isEmpty())
        m_messages.last().content = m_currentAssistantMsg;
    updateMessages();
}

void MainWindow::onResponseFinished() {
    m_sendButton->setEnabled(true);
}

void MainWindow::onErrorOccurred(const QString &error) {
    if (!m_messages.isEmpty())
        m_messages.last().content = "⚠️ " + error;
    updateMessages();
    m_sendButton->setEnabled(true);
}

void MainWindow::onConfigureGroq() {
    QMessageBox intro(this);
    intro.setWindowTitle("Groq API Key");
    intro.setTextFormat(Qt::RichText);
    intro.setText(
        "Groq offers a <b>free</b> API with fast inference.<br><br>"
        "To get a key:<br>"
        "1. Sign up at console.groq.com<br>"
        "2. Go to <b>API Keys</b><br>"
        "3. Create a key and copy it<br><br>"
        "Then paste it in the next dialog."
    );
    intro.setStandardButtons(QMessageBox::Open | QMessageBox::Ok);
    intro.button(QMessageBox::Open)->setText("Open Groq Console");

    if (intro.exec() == QMessageBox::Open) {
        QDesktopServices::openUrl(QUrl("https://console.groq.com/keys"));
    }

    QSettings settings("SupaLLM", "SupaLLMClient");
    bool ok;
    QString key = QInputDialog::getText(this, "Groq API Key",
                                        "Enter your Groq API key:",
                                        QLineEdit::Password,
                                        m_groqApiKey, &ok);
    if (ok && !key.isEmpty()) {
        m_groqApiKey = key;
        settings.setValue("groq/apiKey", key);
        m_groqClient->setApiKey(key);
        QMessageBox::information(this, "Groq", "API key saved.");
    }
}

void MainWindow::onConfigureOllama() {
    OllamaDialog dialog(m_ollamaManager, this);
    dialog.exec();

    // Refresh model list if we're on the Ollama backend
    if (m_backend == Backend::Ollama) {
        m_ollamaManager->refreshInstalledModels();
    }
}

void MainWindow::onThemeSelected(const QString &name) {
    m_themeManager->applyByName(name);
    m_themeManager->saveCurrent();
    pushTheme();
}

void MainWindow::onCustomTheme() {
    ThemeDialog dialog(m_themeManager->currentTheme(), this);

    connect(&dialog, &ThemeDialog::themePreview, this, [this](const Theme &t) {
        m_themeManager->applyTheme(t);
        pushTheme();
    });

    if (dialog.exec() == QDialog::Accepted) {
        Theme chosen = dialog.selectedTheme();

        bool ok;
        QString name = QInputDialog::getText(this, "Save Theme",
                                             "Name this theme (leave blank to apply without saving):",
                                             QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            chosen.name = name;
            m_themeManager->saveCustomTheme(chosen);
        }

        m_themeManager->applyTheme(chosen);
        m_themeManager->saveCurrent();
        pushTheme();
    }
}

void MainWindow::onManageThemes() {
    QList<Theme> customs = m_themeManager->customThemes();

    if (customs.isEmpty()) {
        QMessageBox::information(this, "Manage Themes", "No custom themes saved yet.");
        return;
    }

    QStringList names;
    for (const Theme &t : customs) names << t.name;

    bool ok;
    QString choice = QInputDialog::getItem(this, "Delete Custom Theme",
                                           "Select a theme to delete:",
                                           names, 0, false, &ok);
    if (ok && !choice.isEmpty()) {
        auto reply = QMessageBox::question(this, "Confirm Delete",
                                           "Delete theme \"" + choice + "\"?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            m_themeManager->deleteCustomTheme(choice);
        }
    }
}