#include "ConversationStore.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QVariant>
#include <QDateTime>

ConversationStore::ConversationStore(QObject *parent) : QObject(parent) {
}

bool ConversationStore::init() {
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);
    QString dbPath = dataDir + "/conversations.db";

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning("Could not open conversation database: %s",
                 qPrintable(m_db.lastError().text()));
        return false;
    }

    QSqlQuery query(m_db);

    query.exec(
        "CREATE TABLE IF NOT EXISTS conversations ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  title      TEXT NOT NULL,"
        "  updated_at INTEGER NOT NULL"
        ")"
    );

    query.exec(
        "CREATE TABLE IF NOT EXISTS messages ("
        "  id              INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  conversation_id INTEGER NOT NULL,"
        "  role            INTEGER NOT NULL,"
        "  content         TEXT NOT NULL,"
        "  timestamp       INTEGER NOT NULL,"
        "  FOREIGN KEY(conversation_id) REFERENCES conversations(id) ON DELETE CASCADE"
        ")"
    );

    return true;
}

int ConversationStore::createConversation(const QString &title) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO conversations (title, updated_at) VALUES (?, ?)");
    query.addBindValue(title);
    query.addBindValue(QDateTime::currentSecsSinceEpoch());

    if (!query.exec()) {
        qWarning("createConversation failed: %s", qPrintable(query.lastError().text()));
        return -1;
    }

    return query.lastInsertId().toInt();
}

void ConversationStore::addMessage(int conversationId, const Message &msg) {
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO messages (conversation_id, role, content, timestamp) "
        "VALUES (?, ?, ?, ?)"
    );
    query.addBindValue(conversationId);
    query.addBindValue(static_cast<int>(msg.role));
    query.addBindValue(msg.content);
    query.addBindValue(QDateTime::currentSecsSinceEpoch());
    query.exec();

    // Touch the parent conversation's updated_at
    QSqlQuery touch(m_db);
    touch.prepare("UPDATE conversations SET updated_at = ? WHERE id = ?");
    touch.addBindValue(QDateTime::currentSecsSinceEpoch());
    touch.addBindValue(conversationId);
    touch.exec();
}

void ConversationStore::renameConversation(int id, const QString &title) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE conversations SET title = ? WHERE id = ?");
    query.addBindValue(title);
    query.addBindValue(id);
    query.exec();
}

void ConversationStore::deleteConversation(int id) {
    QSqlQuery msgs(m_db);
    msgs.prepare("DELETE FROM messages WHERE conversation_id = ?");
    msgs.addBindValue(id);
    msgs.exec();

    QSqlQuery conv(m_db);
    conv.prepare("DELETE FROM conversations WHERE id = ?");
    conv.addBindValue(id);
    conv.exec();
}

QList<ConversationMeta> ConversationStore::listConversations() {
    QList<ConversationMeta> result;

    QSqlQuery query(m_db);
    query.exec("SELECT id, title, updated_at FROM conversations ORDER BY updated_at DESC");

    while (query.next()) {
        ConversationMeta meta;
        meta.id        = query.value(0).toInt();
        meta.title     = query.value(1).toString();
        meta.updatedAt = query.value(2).toLongLong();
        result.append(meta);
    }

    return result;
}

QList<ConversationMeta> ConversationStore::searchConversations(const QString &queryStr) {
    QList<ConversationMeta> result;

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT DISTINCT c.id, c.title, c.updated_at "
        "FROM conversations c "
        "LEFT JOIN messages m ON m.conversation_id = c.id "
        "WHERE c.title LIKE ? OR m.content LIKE ? "
        "ORDER BY c.updated_at DESC"
    );
    QString wildcard = "%" + queryStr + "%";
    query.addBindValue(wildcard);
    query.addBindValue(wildcard);
    query.exec();

    while (query.next()) {
        ConversationMeta meta;
        meta.id        = query.value(0).toInt();
        meta.title     = query.value(1).toString();
        meta.updatedAt = query.value(2).toLongLong();
        result.append(meta);
    }

    return result;
}

QList<Message> ConversationStore::loadMessages(int conversationId) {
    QList<Message> result;

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT role, content, timestamp FROM messages "
        "WHERE conversation_id = ? ORDER BY id ASC"
    );
    query.addBindValue(conversationId);
    query.exec();

    while (query.next()) {
        Message msg;
        msg.role      = static_cast<Role>(query.value(0).toInt());
        msg.content   = query.value(1).toString();
        msg.timestamp = query.value(2).toLongLong();
        result.append(msg);
    }

    return result;
}