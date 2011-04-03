#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QString &fileName, QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void saveSettings();

private:
    Ui::SettingsDialog *ui;
    QString m_filename;
};

#endif // SETTINGSDIALOG_H
