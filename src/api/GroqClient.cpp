#include "GroqClient.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrl>

GroqClient::GroqClient(QObject *parent) : QObject(parent) {
    m_network = new QNetworkAccessManager(this);
}

void GroqClient::setApiKey(const QString &key) {
    m_apiKey = key;
}

void GroqClient::sendMessage(const QString &message) {
    m_buffer.clear();

    QNetworkRequest request(QUrl("https://api.groq.com/openai/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + m_apiKey).toUtf8());

    QJsonObject body {
        {"model",  "openai/gpt-oss-20b"},
        {"stream", true},
        {"messages", QJsonArray {
            QJsonObject {
                {"role",    "user"},
                {"content", message}
            }
        }}
    };

    m_reply = m_network->post(request, QJsonDocument(body).toJson());
    connect(m_reply, &QNetworkReply::readyRead, this, &GroqClient::onReadyRead);
    connect(m_reply, &QNetworkReply::finished,  this, &GroqClient::onFinished);
}

void GroqClient::onReadyRead() {
    m_buffer += m_reply->readAll();

    while (true) {
        int newline = m_buffer.indexOf('\n');
        if (newline == -1) break;

        QByteArray line = m_buffer.left(newline).trimmed();
        m_buffer.remove(0, newline + 1);

        if (!line.startsWith("data: ")) continue;
        QByteArray json = line.mid(6);
        if (json == "[DONE]") continue;

        QJsonDocument doc = QJsonDocument::fromJson(json);
        QString token = doc["choices"][0]["delta"]["content"].toString();
        if (!token.isEmpty()) emit tokenReceived(token);
    }
}

void GroqClient::onFinished() {
    int status = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (m_reply->error() != QNetworkReply::NoError) {
        QString errorBody = m_buffer.isEmpty()
            ? m_reply->errorString()
            : QString(m_buffer);

        // Parse the JSON error body for a structured message
        QString detail = errorBody;
        QString code;
        QJsonDocument doc = QJsonDocument::fromJson(errorBody.toUtf8());
        if (doc.isObject()) {
            QJsonObject err = doc["error"].toObject();
            if (!err.isEmpty()) {
                detail = err["message"].toString();
                code   = err["code"].toString();
            }
        }

        if (code == "model_decommissioned") {
            emit errorOccurred(
                "This model has been decommissioned by Groq and is no longer available.\n\n"
                + detail +
                "\n\nUpdate the model in Settings or check https://console.groq.com/docs/models"
            );
        } else if (code == "invalid_api_key") {
            emit errorOccurred("Invalid API key. Check your Groq API key in Settings.");
        } else {
            emit errorOccurred(QString("HTTP %1: %2").arg(status).arg(detail));
        }
    }

    emit responseFinished();
    m_reply->deleteLater();
    m_reply = nullptr;
    m_buffer.clear();
}