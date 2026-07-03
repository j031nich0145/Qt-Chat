#include "MainWindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Supa LLM Client");
    resize(900, 650);

    m_groqClient    = new GroqClient(this);
    m_ollamaManager = new OllamaManager(this);
    m_themeManager  = new ThemeManager(this);
    m_store         = new ConversationStore(this);
    m_store->init();

    setupMenuBar();

    auto *central = new QWidget(this);
    auto *layout  = new QVBoxLayout(central);

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

    connect(m_themeManager, &ThemeManager::customThemesChanged,
            this, &MainWindow::rebuildThemeMenu);
}

void MainWindow::setupMenuBar() {
    QMenu *settingsMenu = menuBar()->addMenu("Settings");

    QAction *modelAction  = new QAction("Backend && Model", this);
    QAction *groqAction   = new QAction("Configure Groq API", this);
    QAction *ollamaAction = new QAction("Configure Ollama", this);

    settingsMenu->addAction(modelAction);
    settingsMenu->addSeparator();
    settingsMenu->addAction(groqAction);
    settingsMenu->addAction(ollamaAction);

    connect(modelAction,  &QAction::triggered, this, &MainWindow::onConfigureModel);
    connect(groqAction,   &QAction::triggered, this, &MainWindow::onConfigureGroq);
    connect(ollamaAction, &QAction::triggered, this, &MainWindow::onConfigureOllama);

    m_historyMenu = menuBar()->addMenu("History");
    rebuildHistoryMenu();

    m_themeMenu = menuBar()->addMenu("Theme");
    rebuildThemeMenu();
}

void MainWindow::rebuildHistoryMenu() {
    m_historyMenu->clear();

    QAction *newChat = new QAction("New Chat", this);
    connect(newChat, &QAction::triggered, this, &MainWindow::onNewChat);
    m_historyMenu->addAction(newChat);

    m_historyMenu->addSeparator();

    QList<ConversationMeta> convs = m_store->listConversations();
    if (convs.isEmpty()) {
        QAction *empty = new QAction("(no saved chats)", this);
        empty->setEnabled(false);
        m_historyMenu->addAction(empty);
    } else {
        for (const ConversationMeta &c : convs) {
            QAction *action = new QAction(c.title, this);
            connect(action, &QAction::triggered, this, [this, id = c.id]() {
                onLoadConversation(id);
            });
            m_historyMenu->addAction(action);
        }
    }

    m_historyMenu->addSeparator();
    QAction *manage = new QAction("Manage Chats...", this);
    connect(manage, &QAction::triggered, this, &MainWindow::onManageHistory);
    m_historyMenu->addAction(manage);
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

QString MainWindow::makeTitle(const QString &firstMessage) {
    QString title = firstMessage.trimmed().simplified();
    if (title.length() > 40)
        title = title.left(40) + "...";
    if (title.isEmpty())
        title = "Chat " + QDateTime::currentDateTime().toString("MMM d, h:mm ap");
    return title;
}

void MainWindow::onNewChat() {
    m_messages.clear();
    m_currentConversationId = -1;
    m_currentAssistantMsg.clear();
    updateMessages();
}

void MainWindow::onLoadConversation(int id) {
    m_messages = m_store->loadMessages(id);
    m_currentConversationId = id;
    m_currentAssistantMsg.clear();
    updateMessages();
}

void MainWindow::onManageHistory() {
    QList<ConversationMeta> convs = m_store->listConversations();
    if (convs.isEmpty()) {
        QMessageBox::information(this, "Manage Chats", "No saved chats yet.");
        return;
    }

    QStringList titles;
    for (const ConversationMeta &c : convs) titles << c.title;

    bool ok;
    QString choice = QInputDialog::getItem(this, "Manage Chats",
                                           "Select a chat:", titles, 0, false, &ok);
    if (!ok || choice.isEmpty()) return;

    int selectedId = -1;
    for (const ConversationMeta &c : convs)
        if (c.title == choice) { selectedId = c.id; break; }
    if (selectedId == -1) return;

    QStringList actions = { "Rename", "Delete" };
    QString action = QInputDialog::getItem(this, "Manage Chats",
                                           "Action for \"" + choice + "\":",
                                           actions, 0, false, &ok);
    if (!ok) return;

    if (action == "Rename") {
        QString newTitle = QInputDialog::getText(this, "Rename Chat",
                                                 "New title:", QLineEdit::Normal,
                                                 choice, &ok);
        if (ok && !newTitle.isEmpty()) {
            m_store->renameConversation(selectedId, newTitle);
            rebuildHistoryMenu();
        }
    } else if (action == "Delete") {
        auto reply = QMessageBox::question(this, "Confirm Delete",
                                           "Delete chat \"" + choice + "\"?",
                                           QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            m_store->deleteConversation(selectedId);
            if (m_currentConversationId == selectedId) onNewChat();
            rebuildHistoryMenu();
        }
    }
}

void MainWindow::onConfigureModel() {
    QDialog dialog(this);
    dialog.setWindowTitle("Backend & Model");
    dialog.setMinimumWidth(360);

    auto *layout = new QVBoxLayout(&dialog);

    layout->addWidget(new QLabel("Backend:", &dialog));
    auto *backendCombo = new QComboBox(&dialog);
    backendCombo->addItem("Groq (cloud)", static_cast<int>(Backend::Groq));
    backendCombo->addItem("Ollama (local)", static_cast<int>(Backend::Ollama));
    backendCombo->setCurrentIndex(static_cast<int>(m_backend));
    layout->addWidget(backendCombo);

    layout->addWidget(new QLabel("Model:", &dialog));
    auto *modelCombo = new QComboBox(&dialog);
    layout->addWidget(modelCombo);

    auto populateModels = [this, modelCombo](Backend backend) {
        modelCombo->clear();
        if (backend == Backend::Groq) {
            modelCombo->addItem("openai/gpt-oss-20b");
            modelCombo->addItem("openai/gpt-oss-120b");
        } else {
            QStringList installed = m_ollamaManager->installedModels();
            if (installed.isEmpty())
                modelCombo->addItem("(no models — see Configure Ollama)");
            else
                modelCombo->addItems(installed);
        }
    };

    populateModels(m_backend);
    if (!m_currentModel.isEmpty()) {
        int idx = modelCombo->findText(m_currentModel);
        if (idx >= 0) modelCombo->setCurrentIndex(idx);
    }

    connect(backendCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            &dialog, [populateModels](int index) {
        populateModels(static_cast<Backend>(index));
    });

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        m_backend = static_cast<Backend>(backendCombo->currentData().toInt());
        m_currentModel = modelCombo->currentText();

        QSettings settings("SupaLLM", "SupaLLMClient");
        settings.setValue("backend/type", static_cast<int>(m_backend));
        settings.setValue("backend/model", m_currentModel);

        if (m_backend == Backend::Ollama)
            m_ollamaManager->refreshInstalledModels();
    }
}

void MainWindow::loadSettings() {
    QSettings settings("SupaLLM", "SupaLLMClient");
    m_groqApiKey = settings.value("groq/apiKey", "").toString();
    m_groqClient->setApiKey(m_groqApiKey);
    m_themeManager->loadSaved();

    m_backend = static_cast<Backend>(
        settings.value("backend/type", static_cast<int>(Backend::Groq)).toInt());
    m_currentModel = settings.value("backend/model", "").toString();
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
        if (msg.content.isEmpty()) continue;
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

    QString model = m_currentModel;
    if (m_backend == Backend::Groq && model.isEmpty())
        model = "openai/gpt-oss-20b";

    if (m_backend == Backend::Ollama &&
        (model.isEmpty() || model.startsWith("("))) {
        QMessageBox::warning(this, "No Model",
            "No Ollama model selected. Open Settings → Backend & Model to pick one.");
        return;
    }

    // Create a conversation on first message
    if (m_currentConversationId == -1) {
        m_currentConversationId = m_store->createConversation(makeTitle(text));
        rebuildHistoryMenu();
    }

    Message userMsg { Role::User, text, QDateTime::currentSecsSinceEpoch() };
    m_messages.append(userMsg);
    m_store->addMessage(m_currentConversationId, userMsg);

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
    // Persist the completed assistant message
    if (!m_messages.isEmpty() && m_currentConversationId != -1) {
        const Message &last = m_messages.last();
        if (last.role == Role::Assistant && !last.content.isEmpty()) {
            m_store->addMessage(m_currentConversationId, last);
        }
    }
    m_sendButton->setEnabled(true);
}

void MainWindow::onErrorOccurred(const QString &error) {
    if (!m_messages.isEmpty())
        m_messages.last().content = "⚠️ " + error;
    updateMessages();
    m_sendButton->setEnabled(true);
}

void MainWindow::onConfigureGroq() {
    QDialog dialog(this);
    dialog.setWindowTitle("Configure Groq API");
    dialog.setMinimumWidth(420);

    auto *layout = new QVBoxLayout(&dialog);

    auto *label = new QLabel("Groq API Key:", &dialog);
    layout->addWidget(label);

    auto *keyField = new QLineEdit(&dialog);
    keyField->setEchoMode(QLineEdit::Password);
    keyField->setPlaceholderText("gsk_...");
    keyField->setText(m_groqApiKey);
    layout->addWidget(keyField);

    auto *guide = new QLabel(&dialog);
    guide->setTextFormat(Qt::RichText);
    guide->setOpenExternalLinks(true);
    guide->setWordWrap(true);
    guide->setText(
        "<br><b>Don't have a key?</b> Groq offers a <b>free</b> API with fast inference.<br><br>"
        "1. Sign up at <a href='https://console.groq.com'>console.groq.com</a><br>"
        "2. Go to <b>API Keys</b><br>"
        "3. Create a key and paste it above"
    );
    layout->addWidget(guide);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString key = keyField->text().trimmed();
        if (!key.isEmpty()) {
            m_groqApiKey = key;
            QSettings settings("SupaLLM", "SupaLLMClient");
            settings.setValue("groq/apiKey", key);
            m_groqClient->setApiKey(key);
            QMessageBox::information(this, "Groq", "API key saved.");
        }
    }
}

void MainWindow::onConfigureOllama() {
    OllamaDialog dialog(m_ollamaManager, this);
    dialog.exec();

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