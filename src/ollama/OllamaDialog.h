#pragma once
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QPlainTextEdit>
#include "OllamaManager.h"

class OllamaDialog : public QDialog {
    Q_OBJECT

public:
    explicit OllamaDialog(OllamaManager *manager, QWidget *parent = nullptr);

private slots:
    void onInstallClicked();
    void onPullClicked();
    void onRemoveClicked();
    void onInstalledModelsChanged(const QStringList &models);
    void onPullProgress(const QString &line);
    void onPullFinished(const QString &tag, bool success);
    void onRemoveFinished(const QString &tag, bool success);

private:
    void refreshState();

    OllamaManager  *m_manager;

    QLabel         *m_statusLabel;
    QPushButton    *m_installButton;
    QComboBox      *m_modelCombo;
    QPushButton    *m_pullButton;
    QListWidget    *m_installedList;
    QPushButton    *m_removeButton;
    QPlainTextEdit *m_logOutput;
};