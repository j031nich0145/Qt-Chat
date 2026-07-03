#include "ThemeDialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QDialogButtonBox>
#include <QFontDatabase>

ThemeDialog::ThemeDialog(const Theme &current, QWidget *parent)
    : QDialog(parent), m_theme(current) {

    setWindowTitle("Custom Theme");
    setMinimumWidth(360);

    auto *mainLayout = new QVBoxLayout(this);
    auto *form = new QFormLayout();

    m_bgSwatch      = makeColorRow("Background", &m_theme.background, form);
    m_surfaceSwatch = makeColorRow("Surface",    &m_theme.surface,    form);
    m_textSwatch    = makeColorRow("Text",       &m_theme.text,       form);
    m_accentSwatch  = makeColorRow("Accent",     &m_theme.accent,     form);

    m_fontCombo = new QComboBox(this);
    m_fontCombo->addItems(QFontDatabase::families());
    int idx = m_fontCombo->findText(m_theme.fontFamily);
    if (idx >= 0) m_fontCombo->setCurrentIndex(idx);
    form->addRow("Font", m_fontCombo);

    m_sizeSpin = new QSpinBox(this);
    m_sizeSpin->setRange(8, 32);
    m_sizeSpin->setValue(m_theme.fontSize);
    m_sizeSpin->setSuffix(" pt");
    form->addRow("Font Size", m_sizeSpin);

    mainLayout->addLayout(form);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Apply | QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        this);
    mainLayout->addWidget(buttons);

    connect(buttons->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &ThemeDialog::onApply);
    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        onApply();
        accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QPushButton *ThemeDialog::makeColorRow(const QString &label, QString *target,
                                       QFormLayout *form) {
    auto *swatch = new QPushButton(this);
    swatch->setFixedSize(60, 24);
    updateSwatch(swatch, *target);

    connect(swatch, &QPushButton::clicked, this, [this, target, swatch]() {
        pickColor(target, swatch);
    });

    form->addRow(label, swatch);
    return swatch;
}

void ThemeDialog::updateSwatch(QPushButton *swatch, const QString &color) {
    swatch->setStyleSheet(
        QString("background-color:%1; border:1px solid #888; border-radius:4px;")
        .arg(color));
    swatch->setText(color);
}

void ThemeDialog::pickColor(QString *target, QPushButton *swatch) {
    QColor initial(*target);
    QColor chosen = QColorDialog::getColor(initial, this, "Pick a color");
    if (chosen.isValid()) {
        *target = chosen.name();
        updateSwatch(swatch, *target);
    }
}

void ThemeDialog::onApply() {
    m_theme.name       = "Custom";
    m_theme.fontFamily = m_fontCombo->currentText();
    m_theme.fontSize   = m_sizeSpin->value();
    emit themePreview(m_theme);
}

Theme ThemeDialog::selectedTheme() const {
    return m_theme;
}