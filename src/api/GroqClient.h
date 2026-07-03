#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QByteArray>

class GroqClient : public QObject {
    Q_OBJECT

public:
    explicit GroqClient(QObject *parent = nullptr);
    void setApiKey(const QString &key);
    void sendMessage(const QString &message);

signals:
    void tokenReceived(const QString &token);
    void responseFinished();
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onFinished();

private:
    QString                m_apiKey;
    QNetworkAccessManager *m_network;
    QNetworkReply         *m_reply = nullptr;
    QByteArray             m_buffer;
};