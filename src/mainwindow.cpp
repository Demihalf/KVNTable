/******************************************************************************
** KVNTable 0.2
** Copyright (C) 2011 Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of KVNTable.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newtabledialog.h"
#include "tablestagestandings.h"
#include "tabletotalstandings.h"
#include <QMessageBox>
#include <QSettings>
#include "settingsdialog.h"
#include <QDate>
#include <QFile>
#include <QDebug>
#include <QCloseEvent>
#include <QDataStream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wndMain)
{
    ui->setupUi(this);

    m_iniFile = qApp->property("iniFile").toString();

    loadSettings();

    addAction(ui->actExitFullscreen);

    connect(ui->actNewTable, SIGNAL(triggered()), SLOT(newTable()));
    connect(ui->actFullscreen, SIGNAL(triggered(bool)),
            SLOT(fullscreenToggled(bool)));
    connect(ui->actSettings, SIGNAL(triggered()), SLOT(showSettingsDlg()));
    connect(ui->actStyleSheetReload, SIGNAL(triggered()),
            SLOT(loadStyleSheet()));
    connect(ui->tabs, SIGNAL(currentChanged(int)),
            SLOT(changeStageTitle(int)));
    connect(ui->actExitFullscreen, SIGNAL(triggered()),
            SLOT(exitFullscreen()));
    connect(ui->actSave, SIGNAL(triggered()), SLOT(saveTable()));
    connect(ui->actOpen, SIGNAL(triggered()), SLOT(openTable()));

    ui->lblDate->setText(QDate(QDate::currentDate()).toString("dd.MM.yyyy"));

    showMaximized();

    ui->statusBar->showMessage("Готов", 1000);
}

void MainWindow::openTable()
{
    QString fileName = QFileDialog
            ::getOpenFileName(this, "Открыть файл таблицы", QString(),
                              "Файлы турнирных таблиц КВН (*.kvn)");

    if (!fileName.isEmpty()) {
        QList<TableData> data;

        QFile f(fileName);
        f.open(QIODevice::ReadOnly);

        QDataStream stream(&f);
        stream >> m_stages;
        stream >> data;

        if (stream.status() == QDataStream::Ok) {
            deleteTabs();

            for (int i = 0; i < data.size() - 1; i++) {
                TableStageStandings *wgt = new TableStageStandings(QStringList(),
                                                                   5, i);
                wgt->setData(data[i]);
                ui->tabs->addTab(wgt, m_stages.at(i));

                connect(wgt, SIGNAL(marksChanged()), SLOT(marksChangedStage()));
                connect(wgt, SIGNAL(teamSectionWidthChanged(int)),
                        SLOT(resizeTeamSections(int)));
            }

            TableTotalStandings *total = new TableTotalStandings(m_stages, QStringList());
            total->setData(data.last());
            ui->tabs->addTab(total, "Общий итог");
        } else {
            QMessageBox::critical(this, "Ошибка!", "Выбранный вами файл не "
                                  "является корректным файлом турннирной "
                                  "таблицы КВН!");
        }
    }
}

void MainWindow::saveTable()
{
    QString fileName = QFileDialog
            ::getSaveFileName(this, "Сохранить файл таблицы", QString(),
                              "Файлы турнирных таблиц КВН (*.kvn)");
    if (!fileName.isEmpty()) {
        QList<TableData> data;

        for (int i = 0; i < ui->tabs->count(); i++) {
            data << qobject_cast<TableStandings *>(ui->tabs->widget(i))->getData();
        }

        QFileInfo info(fileName);

        if (info.suffix().isEmpty()) {
            fileName += ".kvn";
        }

        QFile f(fileName);
        f.open(QIODevice::WriteOnly);

        QDataStream stream(&f);
        stream << m_stages;
        stream << data;
    }

}

void MainWindow::changeStageTitle(int curr)
{
    ui->lblStageName->setText(ui->tabs->tabText(curr));
}

void MainWindow::loadStyleSheet()
{
    QFile style("style.css");

    if (style.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qApp->setStyleSheet(style.readAll());
    }
}

void MainWindow::loadSettings()
{
    QSettings sets(m_iniFile, QSettings::IniFormat);
    ui->lblTitle->setText(sets.value("title_text").toString());
}

void MainWindow::showSettingsDlg()
{
    SettingsDialog *dlg = new SettingsDialog(this);

    if (dlg->exec()) {
        loadSettings();
    }

    delete dlg;
}

void MainWindow::newTable()
{
    const int numOfJudges = 5;

    NewTableDialog *dialog = new NewTableDialog(this);
    if (dialog->exec()) {
        deleteTabs();

        QStringList teams = dialog->teams();
        m_stages = dialog->stages();

        for (int i = 0; i < m_stages.size(); i++) {
            TableStageStandings *wgt = new TableStageStandings(teams,
                                                               numOfJudges, i);
            ui->tabs->addTab(wgt, m_stages.at(i));

            connect(wgt, SIGNAL(marksChanged()), SLOT(marksChangedStage()));
            connect(wgt, SIGNAL(teamSectionWidthChanged(int)),
                    SLOT(resizeTeamSections(int)));
        }

        TableTotalStandings *total = new TableTotalStandings(m_stages, teams);
        ui->tabs->addTab(total, "Общий итог");

        connect(total, SIGNAL(teamSectionWidthChanged(int)),
                SLOT(resizeTeamSections(int)));
    }

    ui->tabs->setCurrentIndex(0);
    changeStageTitle(0);

    delete dialog;
}

void MainWindow::deleteTabs()
{
    while (ui->tabs->count()) {
        QWidget *tmp = ui->tabs->widget(0);
        ui->tabs->removeTab(0);

        Q_ASSERT(tmp);
        delete tmp;
    }
}

void MainWindow::marksChangedStage()
{
    int tabsCount = ui->tabs->count();

    TableTotalStandings *total = qobject_cast<TableTotalStandings *>
            (ui->tabs->widget(tabsCount - 1));

    Q_ASSERT(total);

    // marks[stage][team]
    QList<QList<double> > allMarks;

    for (int stage = 0; stage < tabsCount - 1; ++stage) {
        TableStageStandings *wgt = qobject_cast<TableStageStandings *>
                (ui->tabs->widget(stage));
        Q_ASSERT(wgt);
        allMarks << wgt->averages();
    }

    total->setMarks(allMarks);

    // Intermediate marks
    for (int stage = 1; stage < tabsCount - 1; stage++) {
        TableStageStandings *wgt = qobject_cast<TableStageStandings *>
                (ui->tabs->widget(stage));

        int teams = wgt->averages().size();
        QList<double> sums;

        for (int i = 0; i < teams; i++) {
            double sum = 0;

            for (int j = 0; j <= stage; j++) {
                sum += allMarks[j][i];
            }

            sums << sum;
        }

        wgt->setIntermediateResults(sums);
    }
}

void MainWindow::fullscreenToggled(bool isActivated)
{
    if (isActivated) {
        wasMaximized = isMaximized();
        showFullScreen();
        ui->mainToolBar->hide();
    } else {
        showNormal();
        if (wasMaximized) {
            showMaximized();
        }
        ui->mainToolBar->show();
    }
}

void MainWindow::exitFullscreen()
{
    if (isFullScreen()) {
        ui->actFullscreen->trigger();
    }
}

void MainWindow::resizeTeamSections(int newSize)
{
    for (int i = 0; i < ui->tabs->count(); ++i) {
        TableStandings *tmp = qobject_cast<TableStandings *>
                (ui->tabs->widget(i));
        Q_ASSERT(tmp);

        // To prevent recursion
        disconnect(tmp, SIGNAL(teamSectionWidthChanged(int)), this,
                   SLOT(resizeTeamSections(int)));

        tmp->resizeTeamSection(newSize);

        connect(tmp, SIGNAL(teamSectionWidthChanged(int)),
                SLOT(resizeTeamSections(int)));
    }
}

MainWindow::~MainWindow()
{
    deleteTabs();
    delete ui;
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::about()
{
    QFile f(":/about_string.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&f);

    QString about = stream.readAll().arg(qApp->applicationName())
            .arg(qApp->applicationVersion()).replace("\n", " ");

    QMessageBox::about(this, "О программе", about);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton btn =
            QMessageBox::question(this, "Выход", "Вы действительно хотите выйти?",
                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (btn == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
