#include "OllamaManager.h"
#include <QStandardPaths>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>

OllamaManager::OllamaManager(QObject *parent) : QObject(parent) {
    m_network = new QNetworkAccessManager(this);
}

QList<OllamaModel> OllamaManager::curatedModels() {
    return {
        { "gemma3:1b",  "Gemma 3 1B",  "~0.8 GB", "fastest" },
        { "qwen3:1.7b", "Qwen3 1.7B",  "~1.4 GB", "balanced" },
        { "phi4-mini",  "Phi-4 Mini",  "~2.3 GB", "most capable" }
    };
}

bool OllamaManager::isInstalled() const {
    return !QStandardPaths::findExecutable("ollama").isEmpty();
}

void OllamaManager::refreshInstalledModels() {
    if (!isInstalled()) {
        m_installed.clear();
        emit installedModelsChanged(m_installed);
        return;
    }

    auto *proc = new QProcess(this);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, proc](int, QProcess::ExitStatus) {
        QString output = proc->readAllStandardOutput();
        m_installed.clear();

        const QStringList lines = output.split('\n', Qt::SkipEmptyParts);
        for (int i = 1; i < lines.size(); ++i) {  // skip header row
            QString name = lines[i].section(' ', 0, 0).trimmed();
            if (!name.isEmpty()) m_installed << name;
        }

        emit installedModelsChanged(m_installed);
        proc->deleteLater();
    });

    proc->start("ollama", { "list" });
}

QStringList OllamaManager::installedModels() const {
    return m_installed;
}

void OllamaManager::pullModel(const QString &tag) {
    auto *proc = new QProcess(this);

    connect(proc, &QProcess::readyReadStandardError, this, [this, proc]() {
        emit pullProgress(proc->readAllStandardError());
    });
    connect(proc, &QProcess::readyReadStandardOutput, this, [this, proc]() {
        emit pullProgress(proc->readAllStandardOutput());
    });
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, proc, tag](int code, QProcess::ExitStatus) {
        emit pullFinished(tag, code == 0);
        refreshInstalledModels();
        proc->deleteLater();
    });

    proc->start("ollama", { "pull", tag });
}

void OllamaManager::removeModel(const QString &tag) {
    auto *proc = new QProcess(this);

    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, proc, tag](int code, QProcess::ExitStatus) {
        emit removeFinished(tag, code == 0);
        refreshInstalledModels();
        proc->deleteLater();
    });

    proc->start("ollama", { "rm", tag });
}

void OllamaManager::sendMessage(const QString &model, const QJsonArray &messages) {
    m_buffer.clear();

    QNetworkRequest request(QUrl("http://localhost:11434/api/chat"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body {
        {"model",    model},
        {"stream",   true},
        {"messages", messages}
    };

    m_reply = m_network->post(request, QJsonDocument(body).toJson());
    connect(m_reply, &QNetworkReply::readyRead, this, &OllamaManager::onReadyRead);
    connect(m_reply, &QNetworkReply::finished,  this, &OllamaManager::onFinished);
}

void OllamaManager::onReadyRead() {
    m_buffer += m_reply->readAll();

    // Ollama streams newline-delimited JSON objects (one per line)
    while (true) {
        int newline = m_buffer.indexOf('\n');
        if (newline == -1) break;

        QByteArray line = m_buffer.left(newline).trimmed();
        m_buffer.remove(0, newline + 1);
        if (line.isEmpty()) continue;

        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (!doc.isObject()) continue;

        QJsonObject obj = doc.object();
        QString token = obj["message"].toObject()["content"].toString();
        if (!token.isEmpty()) emit tokenReceived(token);
    }
}

void OllamaManager::onFinished() {
    if (m_reply->error() != QNetworkReply::NoError) {
        int status = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString detail = m_buffer.isEmpty() ? m_reply->errorString() : QString(m_buffer);

        if (m_reply->error() == QNetworkReply::ConnectionRefusedError) {
            emit errorOccurred("Cannot reach Ollama. Is it running? Try `ollama serve` in a terminal.");
        } else {
            emit errorOccurred(QString("Ollama HTTP %1: %2").arg(status).arg(detail));
        }
    }

    emit responseFinished();
    m_reply->deleteLater();
    m_reply = nullptr;
    m_buffer.clear();
}