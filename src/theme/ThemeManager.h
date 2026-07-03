#pragma once
#include <QObject>
#include <QList>
#include <QString>
#include "Theme.h"

class ThemeManager : public QObject {
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);

    QList<Theme> presets() const;
    QList<Theme> customThemes() const;
    Theme        currentTheme() const;

    void applyTheme(const Theme &theme);
    void applyByName(const QString &name);

    void saveCustomTheme(const Theme &theme);
    void deleteCustomTheme(const QString &name);

    void loadSaved();
    void saveCurrent();
    void loadCustomThemes();

    static QString buildStyleSheet(const Theme &theme);

signals:
    void themeChanged(const Theme &theme);
    void customThemesChanged();

private:
    Theme *findTheme(const QString &name);

    QList<Theme> m_presets;
    QList<Theme> m_customThemes;
    Theme        m_current;
};