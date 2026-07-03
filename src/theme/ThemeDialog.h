#pragma once
#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include "Theme.h"

class ThemeDialog : public QDialog {
    Q_OBJECT

public:
    explicit ThemeDialog(const Theme &current, QWidget *parent = nullptr);
    Theme selectedTheme() const;

signals:
    void themePreview(const Theme &theme);

private slots:
    void pickColor(QString *target, QPushButton *swatch);
    void onApply();

private:
    void updateSwatch(QPushButton *swatch, const QString &color);
    QPushButton *makeColorRow(const QString &label, QString *target,
                              class QFormLayout *form);

    Theme m_theme;

    QPushButton *m_bgSwatch;
    QPushButton *m_surfaceSwatch;
    QPushButton *m_textSwatch;
    QPushButton *m_accentSwatch;
    QComboBox   *m_fontCombo;
    QSpinBox    *m_sizeSpin;
};