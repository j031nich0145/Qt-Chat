#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QList>
#include "../models/Message.h"

struct ConversationMeta {
    int     id;
    QString title;
    qint64  updatedAt;
};

class ConversationStore : public QObject {
    Q_OBJECT

public:
    explicit ConversationStore(QObject *parent = nullptr);
    bool init();

    int  createConversation(const QString &title);
    void addMessage(int conversationId, const Message &msg);
    void renameConversation(int id, const QString &title);
    void deleteConversation(int id);

    QList<ConversationMeta> listConversations();
    QList<ConversationMeta> searchConversations(const QString &query);
    QList<Message>          loadMessages(int conversationId);

private:
    QSqlDatabase m_db;
};