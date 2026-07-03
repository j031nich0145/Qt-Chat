#include "ThemeManager.h"
#include <QApplication>
#include <QSettings>

ThemeManager::ThemeManager(QObject *parent) : QObject(parent) {
    m_presets = {
        {
            "Dark",
            "#1e1e2e", "#313244", "#cdd6f4", "#89b4fa",
            "Sans Serif", 11
        },
        {
            "Light",
            "#eff1f5", "#ffffff", "#4c4f69", "#1e66f5",
            "Sans Serif", 11
        },
        {
            "Solarized",
            "#002b36", "#073642", "#93a1a1", "#268bd2",
            "Sans Serif", 11
        },
        {
            "Nord",
            "#2e3440", "#3b4252", "#eceff4", "#88c0d0",
            "Sans Serif", 11
        },
        {
            "Dracula",
            "#282a36", "#44475a", "#f8f8f2", "#bd93f9",
            "Sans Serif", 11
        }
    };

    m_current = m_presets.first();
    loadCustomThemes();
}

QList<Theme> ThemeManager::presets() const {
    return m_presets;
}

QList<Theme> ThemeManager::customThemes() const {
    return m_customThemes;
}

Theme ThemeManager::currentTheme() const {
    return m_current;
}

Theme *ThemeManager::findTheme(const QString &name) {
    for (Theme &t : m_presets)
        if (t.name == name) return &t;
    for (Theme &t : m_customThemes)
        if (t.name == name) return &t;
    return nullptr;
}

void ThemeManager::applyTheme(const Theme &theme) {
    m_current = theme;
    qApp->setStyleSheet(buildStyleSheet(theme));
    emit themeChanged(theme);
}

void ThemeManager::applyByName(const QString &name) {
    Theme *t = findTheme(name);
    if (t) applyTheme(*t);
}

void ThemeManager::saveCustomTheme(const Theme &theme) {
    // Replace if a custom theme with this name already exists
    for (int i = 0; i < m_customThemes.size(); ++i) {
        if (m_customThemes[i].name == theme.name) {
            m_customThemes[i] = theme;
            QSettings settings("SupaLLM", "SupaLLMClient");
            settings.beginGroup("customThemes");
            settings.beginGroup(theme.name);
            settings.setValue("background", theme.background);
            settings.setValue("surface",    theme.surface);
            settings.setValue("text",       theme.text);
            settings.setValue("accent",     theme.accent);
            settings.setValue("fontFamily", theme.fontFamily);
            settings.setValue("fontSize",   theme.fontSize);
            settings.endGroup();
            settings.endGroup();
            emit customThemesChanged();
            return;
        }
    }

    m_customThemes.append(theme);

    QSettings settings("SupaLLM", "SupaLLMClient");
    settings.beginGroup("customThemes");
    settings.beginGroup(theme.name);
    settings.setValue("background", theme.background);
    settings.setValue("surface",    theme.surface);
    settings.setValue("text",       theme.text);
    settings.setValue("accent",     theme.accent);
    settings.setValue("fontFamily", theme.fontFamily);
    settings.setValue("fontSize",   theme.fontSize);
    settings.endGroup();
    settings.endGroup();

    emit customThemesChanged();
}

void ThemeManager::deleteCustomTheme(const QString &name) {
    for (int i = 0; i < m_customThemes.size(); ++i) {
        if (m_customThemes[i].name == name) {
            m_customThemes.removeAt(i);
            break;
        }
    }

    QSettings settings("SupaLLM", "SupaLLMClient");
    settings.beginGroup("customThemes");
    settings.remove(name);
    settings.endGroup();

    emit customThemesChanged();
}

void ThemeManager::loadCustomThemes() {
    m_customThemes.clear();

    QSettings settings("SupaLLM", "SupaLLMClient");
    settings.beginGroup("customThemes");
    const QStringList names = settings.childGroups();

    for (const QString &name : names) {
        settings.beginGroup(name);
        Theme t;
        t.name       = name;
        t.background = settings.value("background").toString();
        t.surface    = settings.value("surface").toString();
        t.text       = settings.value("text").toString();
        t.accent     = settings.value("accent").toString();
        t.fontFamily = settings.value("fontFamily").toString();
        t.fontSize   = settings.value("fontSize").toInt();
        settings.endGroup();
        m_customThemes.append(t);
    }

    settings.endGroup();
}

void ThemeManager::loadSaved() {
    QSettings settings("SupaLLM", "SupaLLMClient");
    if (!settings.contains("theme/name")) {
        applyTheme(m_current);
        return;
    }

    Theme t;
    t.name       = settings.value("theme/name").toString();
    t.background = settings.value("theme/background").toString();
    t.surface    = settings.value("theme/surface").toString();
    t.text       = settings.value("theme/text").toString();
    t.accent     = settings.value("theme/accent").toString();
    t.fontFamily = settings.value("theme/fontFamily").toString();
    t.fontSize   = settings.value("theme/fontSize").toInt();

    applyTheme(t);
}

void ThemeManager::saveCurrent() {
    QSettings settings("SupaLLM", "SupaLLMClient");
    settings.setValue("theme/name",       m_current.name);
    settings.setValue("theme/background", m_current.background);
    settings.setValue("theme/surface",    m_current.surface);
    settings.setValue("theme/text",       m_current.text);
    settings.setValue("theme/accent",     m_current.accent);
    settings.setValue("theme/fontFamily", m_current.fontFamily);
    settings.setValue("theme/fontSize",   m_current.fontSize);
}

QString ThemeManager::buildStyleSheet(const Theme &theme) {
    return QString(
        "QMainWindow, QWidget {"
        "  background-color: %1;"
        "  color: %3;"
        "  font-family: '%5';"
        "  font-size: %6pt;"
        "}"
        "QTextEdit, QLineEdit {"
        "  background-color: %2;"
        "  color: %3;"
        "  border: 1px solid %4;"
        "  border-radius: 6px;"
        "  padding: 6px;"
        "}"
        "QPushButton {"
        "  background-color: %4;"
        "  color: %1;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 6px 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: %3;"
        "}"
        "QPushButton:disabled {"
        "  background-color: %2;"
        "  color: %4;"
        "}"
        "QMenuBar, QMenu {"
        "  background-color: %2;"
        "  color: %3;"
        "}"
        "QMenu::item:selected, QMenuBar::item:selected {"
        "  background-color: %4;"
        "  color: %1;"
        "}"
    )
    .arg(theme.background)
    .arg(theme.surface)
    .arg(theme.text)
    .arg(theme.accent)
    .arg(theme.fontFamily)
    .arg(theme.fontSize);
}