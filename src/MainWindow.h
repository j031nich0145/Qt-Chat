#pragma once
#include <QMainWindow>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QList>
#include "api/GroqClient.h"
#include "ollama/OllamaManager.h"
#include "ollama/OllamaDialog.h"
#include "theme/ThemeManager.h"
#include "theme/ThemeDialog.h"
#include "models/Message.h"

enum class Backend { Groq, Ollama };

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSendClicked();
    void onConfigureGroq();
    void onConfigureOllama();
    void onTokenReceived(const QString &token);
    void onResponseFinished();
    void onErrorOccurred(const QString &error);
    void onThemeSelected(const QString &name);
    void onCustomTheme();
    void onManageThemes();
    void onPageLoaded(bool ok);
    void onBackendChanged(int index);
    void onOllamaModelsChanged(const QStringList &models);

private:
    void setupMenuBar();
    QWidget *setupBackendBar();
    void rebuildThemeMenu();
    void loadSettings();
    void updateMessages();
    void pushTheme();
    QJsonArray buildMessageHistory();

    QWebEngineView *m_chatView;
    QLineEdit      *m_inputField;
    QPushButton    *m_sendButton;
    QComboBox      *m_backendCombo;
    QComboBox      *m_modelCombo;
    QString         m_groqApiKey;
    GroqClient     *m_groqClient;
    OllamaManager  *m_ollamaManager;
    ThemeManager   *m_themeManager;
    QMenu          *m_themeMenu = nullptr;
    QList<Message>  m_messages;
    QString         m_currentAssistantMsg;
    Backend         m_backend = Backend::Groq;
    bool            m_pageReady = false;
};