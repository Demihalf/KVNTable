#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>

SettingsDialog::SettingsDialog(const QString &fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog), m_filename(fileName)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(saveSettings()));

    QSettings sets(m_filename, QSettings::IniFormat);
    ui->txtTitle->setText(sets.value("title_text").toString());
}

void SettingsDialog::saveSettings()
{
    QSettings sets(m_filename, QSettings::IniFormat);
    sets.setValue("title_text", ui->txtTitle->document()->toPlainText());
    accept();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
