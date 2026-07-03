#include "OllamaDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

OllamaDialog::OllamaDialog(OllamaManager *manager, QWidget *parent)
    : QDialog(parent), m_manager(manager) {

    setWindowTitle("Configure Ollama");
    setMinimumWidth(480);

    auto *layout = new QVBoxLayout(this);

    // ── Install status ─────────────────────────────────────────
    auto *statusRow = new QHBoxLayout();
    m_statusLabel = new QLabel(this);
    m_installButton = new QPushButton("Install Ollama", this);
    statusRow->addWidget(m_statusLabel, 1);
    statusRow->addWidget(m_installButton);
    layout->addLayout(statusRow);

    // ── Download models ────────────────────────────────────────
    auto *pullGroup = new QGroupBox("Download a Model", this);
    auto *pullLayout = new QHBoxLayout(pullGroup);
    m_modelCombo = new QComboBox(this);
    for (const OllamaModel &m : OllamaManager::curatedModels()) {
        QString display = QString("%1  (%2, %3)")
            .arg(m.label).arg(m.note).arg(m.size);
        m_modelCombo->addItem(display, m.tag);
    }
    m_pullButton = new QPushButton("Download", this);
    pullLayout->addWidget(m_modelCombo, 1);
    pullLayout->addWidget(m_pullButton);
    layout->addWidget(pullGroup);

    // ── Installed models ───────────────────────────────────────
    auto *installedGroup = new QGroupBox("Installed Models", this);
    auto *installedLayout = new QVBoxLayout(installedGroup);
    m_installedList = new QListWidget(this);
    m_removeButton = new QPushButton("Remove Selected", this);
    installedLayout->addWidget(m_installedList);
    installedLayout->addWidget(m_removeButton);
    layout->addWidget(installedGroup);

    // ── Log output ─────────────────────────────────────────────
    m_logOutput = new QPlainTextEdit(this);
    m_logOutput->setReadOnly(true);
    m_logOutput->setMaximumHeight(100);
    m_logOutput->setPlaceholderText("Download progress will appear here...");
    layout->addWidget(m_logOutput);

    // ── Connections ────────────────────────────────────────────
    connect(m_installButton, &QPushButton::clicked, this, &OllamaDialog::onInstallClicked);
    connect(m_pullButton,    &QPushButton::clicked, this, &OllamaDialog::onPullClicked);
    connect(m_removeButton,  &QPushButton::clicked, this, &OllamaDialog::onRemoveClicked);

    connect(m_manager, &OllamaManager::installedModelsChanged,
            this, &OllamaDialog::onInstalledModelsChanged);
    connect(m_manager, &OllamaManager::pullProgress, this, &OllamaDialog::onPullProgress);
    connect(m_manager, &OllamaManager::pullFinished, this, &OllamaDialog::onPullFinished);
    connect(m_manager, &OllamaManager::removeFinished, this, &OllamaDialog::onRemoveFinished);

    refreshState();
    m_manager->refreshInstalledModels();
}

void OllamaDialog::refreshState() {
    bool installed = m_manager->isInstalled();

    if (installed) {
        m_statusLabel->setText("✅ Ollama is installed.");
        m_installButton->setEnabled(false);
    } else {
        m_statusLabel->setText("⚠️ Ollama is not installed.");
        m_installButton->setEnabled(true);
    }

    m_modelCombo->setEnabled(installed);
    m_pullButton->setEnabled(installed);
    m_installedList->setEnabled(installed);
    m_removeButton->setEnabled(installed);
}

void OllamaDialog::onInstallClicked() {
    QMessageBox box(this);
    box.setWindowTitle("Install Ollama");
    box.setTextFormat(Qt::RichText);
    box.setText(
        "Ollama needs to be installed on your system.<br><br>"
        "On Linux, run this in a terminal:<br>"
        "<code>curl -fsSL https://ollama.com/install.sh | sh</code><br><br>"
        "Or visit the download page for other systems."
    );
    box.setStandardButtons(QMessageBox::Open | QMessageBox::Close);
    box.button(QMessageBox::Open)->setText("Open Download Page");

    if (box.exec() == QMessageBox::Open) {
        QDesktopServices::openUrl(QUrl("https://ollama.com/download"));
    }
}

void OllamaDialog::onPullClicked() {
    QString tag = m_modelCombo->currentData().toString();
    if (tag.isEmpty()) return;

    m_logOutput->clear();
    m_logOutput->appendPlainText("Downloading " + tag + "...");
    m_pullButton->setEnabled(false);

    m_manager->pullModel(tag);
}

void OllamaDialog::onRemoveClicked() {
    QListWidgetItem *item = m_installedList->currentItem();
    if (!item) {
        QMessageBox::information(this, "Remove Model", "Select a model to remove first.");
        return;
    }

    QString tag = item->text();
    auto reply = QMessageBox::question(this, "Confirm Remove",
                                       "Remove model \"" + tag + "\"?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        m_manager->removeModel(tag);
    }
}

void OllamaDialog::onInstalledModelsChanged(const QStringList &models) {
    m_installedList->clear();
    m_installedList->addItems(models);
}

void OllamaDialog::onPullProgress(const QString &line) {
    m_logOutput->appendPlainText(line.trimmed());
}

void OllamaDialog::onPullFinished(const QString &tag, bool success) {
    m_pullButton->setEnabled(true);
    if (success) {
        m_logOutput->appendPlainText("✅ " + tag + " downloaded successfully.");
    } else {
        m_logOutput->appendPlainText("❌ Failed to download " + tag + ".");
    }
}

void OllamaDialog::onRemoveFinished(const QString &tag, bool success) {
    if (success) {
        m_logOutput->appendPlainText("Removed " + tag + ".");
    } else {
        m_logOutput->appendPlainText("Failed to remove " + tag + ".");
    }
}