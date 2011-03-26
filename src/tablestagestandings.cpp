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

#include "tablestagestandings.h"
#include <QHeaderView>
#include <QResizeEvent>

TableStageStandings::TableStageStandings(const QStringList &teams, int numOfMarks,
                               QWidget *parent) :
    TableStandings(teams, numOfMarks, parent)
{
    createHeader();

    connect(this, SIGNAL(cellChanged(int,int)), SLOT(recalculateAverage()));
}

void TableStageStandings::createHeader()
{
    QStringList headerLabels;
    headerLabels << "Команда";

    for (int i = 1; i <= m_numOfMiddleCells; i++) {
        headerLabels << QString("%1").arg(i);
    }

    headerLabels << "Средний балл";
    setHorizontalHeaderLabels(headerLabels);

    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);

    verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void TableStageStandings::recalculateAverage()
{
    QList<double> avs = averages();

    for (int i = 0; i < rowCount(); i++) {
        item(i, columnCount() - 1)->setText(QString("%1").arg(avs[i]));
    }

    emit marksChanged();
}

QList<double> TableStageStandings::averages()
{
    QList<double> avs;

    for (int i = 0; i < rowCount(); i++) {
        avs << averageAt(i);
    }

    return avs;
}

double TableStageStandings::averageAt(int row)
{
    double sum = 0;
    for (int i = 1; i < columnCount() - 1; i++) {
        sum += item(row, i)->text().toDouble();
    }

    return sum / double(columnCount() - 2);
}

TableStageStandings::~TableStageStandings()
{
}
