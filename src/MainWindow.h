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
#include "db/ConversationStore.h"
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
    void onConfigureModel();
    void onTokenReceived(const QString &token);
    void onResponseFinished();
    void onErrorOccurred(const QString &error);
    void onThemeSelected(const QString &name);
    void onCustomTheme();
    void onManageThemes();
    void onPageLoaded(bool ok);
    void onNewChat();
    void onLoadConversation(int id);
    void onManageHistory();

private:
    void setupMenuBar();
    void rebuildThemeMenu();
    void rebuildHistoryMenu();
    void loadSettings();
    void updateMessages();
    void pushTheme();
    QJsonArray buildMessageHistory();
    QString makeTitle(const QString &firstMessage);

    QWebEngineView    *m_chatView;
    QLineEdit         *m_inputField;
    QPushButton       *m_sendButton;
    QString            m_groqApiKey;
    GroqClient        *m_groqClient;
    OllamaManager     *m_ollamaManager;
    ThemeManager      *m_themeManager;
    ConversationStore *m_store;
    QMenu             *m_themeMenu = nullptr;
    QMenu             *m_historyMenu = nullptr;
    QList<Message>     m_messages;
    QString            m_currentAssistantMsg;
    QString            m_currentModel;
    Backend            m_backend = Backend::Groq;
    int                m_currentConversationId = -1;
    bool               m_pageReady = false;
};