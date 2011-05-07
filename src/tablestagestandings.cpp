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

#include "tablestagestandings.h"
#include <QHeaderView>
#include <QResizeEvent>

TableStageStandings::TableStageStandings(const QStringList &teams, int numOfMarks,
                                         int stageNumber, QWidget *parent) :
    TableStandings(teams, numOfMarks, parent), m_stageNumber(stageNumber)
{
    setColumnCount(m_numOfMiddleCells + 3 + (m_stageNumber != 0 ? 1 : 0));

    createHeader();
    createCells();

    connect(this, SIGNAL(cellChanged(int,int)), SLOT(recalculateAverage()));
}

void TableStageStandings::createHeader()
{
    QStringList headerLabels;
    headerLabels << "Команда";

    for (int i = 1; i <= m_numOfMiddleCells; i++) {
        headerLabels << QString("%1").arg(i);
    }

    headerLabels << "Штраф" << "Балл";

    if (m_stageNumber != 0) {
        headerLabels << "Результат";
    }

    setHorizontalHeaderLabels(headerLabels);

    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);

    verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

void TableStageStandings::setIntermediateResults(const QList<double> &mark)
{
    for (int row = 0; row < m_teams.size(); row++) {
        item(row, columnCount() - 1)->setText(QString("%1").arg(mark[row]));
    }
}

void TableStageStandings::createLastCells()
{
    for (int row = 0; row < m_teams.size(); row++) {
        for (int col = m_numOfMiddleCells + 1; col < columnCount(); col++) {
            QTableWidgetItem * it = new QTableWidgetItem();
            it->setTextAlignment(Qt::AlignCenter);

            setItem(row, col, it);
        }

        // Average mark
        item(row, m_numOfMiddleCells + 2)->setFlags(Qt::ItemIsSelectable |
                                                    Qt::ItemIsEnabled);

        // Intermediate result
        if (m_stageNumber != 0) {
            item(row, m_numOfMiddleCells + 3)->setFlags(Qt::ItemIsSelectable |
                                                        Qt::ItemIsEnabled);
        }
    }
}

void TableStageStandings::recalculateAverage()
{
    QList<double> avs = averages();

    for (int i = 0; i < rowCount(); i++) {
        item(i, m_numOfMiddleCells + 2)->setText(QString("%1").arg(avs[i]));
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
    for (int i = 1; i <= m_numOfMiddleCells; i++) {
        sum += item(row, i)->text().toDouble();
    }

    double penalty = item(row, m_numOfMiddleCells + 1)->text().toDouble();
    double avrg = sum / double(m_numOfMiddleCells);

    avrg -= penalty;

    return avrg;
}

TableStageStandings::~TableStageStandings()
{
}
