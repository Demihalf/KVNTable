/******************************************************************************
** KVNTable 0.1
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wndMain)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Готов", 1000);

    connect(ui->actNewTable, SIGNAL(triggered()), SLOT(newTable()));
}

void MainWindow::newTable()
{
    const int numOfJudges = 5;

    NewTableDialog *dialog = new NewTableDialog(this);
    if (dialog->exec()) {
        deleteTabs();

        QStringList teams = dialog->teams();
        QStringList stages = dialog->stages();

        foreach (QString stage, stages) {
            TableStageStandings *wgt = new TableStageStandings(teams,
                                                               numOfJudges);
            ui->tabs->addTab(wgt, stage);

            connect(wgt, SIGNAL(marksChanged()), SLOT(marksChangedStage()));
        }

        TableTotalStandings *total = new TableTotalStandings(stages, teams);
        ui->tabs->addTab(total, "Общий итог");
    }
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
    QMessageBox::about(this, "О программе", "<p>Программа KVNTable предназначена "
                       "для создания турнирных таблиц игр КВН. По всем вопросам, "
                       "касающимся этой программы вы можете обращаться по email "
                       "<a href=\"mailto:kharvd@gmail.com\">kharvd@gmail.com</a></p>"

                       "<p>Copyright © 2011 Валерий Харитонов</p>"

                       "<p>Это программа распространяется БЕЗ ВСЯКИХ ГАРАНТИЙ. "
                       "Это свободное программное обеспечение, и Вы можете "
                       "распространять её в соответствии с конкретными условиями. "
                       "Для дополнительной информации смотрите "
                       "<a href=\"http://www.gnu.org/licenses/gpl.html\">"
                       "http://www.gnu.org/licenses/gpl.html</a></p>");
}
