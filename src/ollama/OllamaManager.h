#pragma once
#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

struct OllamaModel {
    QString tag;
    QString label;
    QString size;
    QString note;
};

class OllamaManager : public QObject {
    Q_OBJECT

public:
    explicit OllamaManager(QObject *parent = nullptr);

    static QList<OllamaModel> curatedModels();

    bool isInstalled() const;
    void refreshInstalledModels();
    QStringList installedModels() const;

    void pullModel(const QString &tag);
    void removeModel(const QString &tag);

    void sendMessage(const QString &model, const QJsonArray &messages);

signals:
    void installedModelsChanged(const QStringList &models);
    void pullProgress(const QString &line);
    void pullFinished(const QString &tag, bool success);
    void removeFinished(const QString &tag, bool success);

    void tokenReceived(const QString &token);
    void responseFinished();
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onFinished();

private:
    QStringList            m_installed;
    QNetworkAccessManager *m_network;
    QNetworkReply         *m_reply = nullptr;
    QByteArray             m_buffer;
};