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

#include "tablestandings.h"
#include <QHeaderView>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>

TableStandings::TableStandings(const QStringList &teams, int middleCells,
                               QWidget *parent) :
    QTableWidget(parent), m_teams(teams), m_numOfMiddleCells(middleCells)
{
    setColumnCount(m_numOfMiddleCells + 2);

    createTeams();
    createCells();

    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->hide();

    setAlternatingRowColors(true);
}

void TableStandings::createTeams()
{
    foreach (QString team, m_teams) {
        int rows = rowCount();
        QTableWidgetItem * it = new QTableWidgetItem(team);

        it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        it->setTextAlignment(Qt::AlignCenter);

        insertRow(rows);
        setItem(rows, 0, it);
    }
}

void TableStandings::createCells()
{
    int lastCol = columnCount() - 1;

    for (int row = 0; row < m_teams.size(); row++) {
        for (int col = 1; col <= m_numOfMiddleCells + 1; col++) {
            QTableWidgetItem * it = new QTableWidgetItem();

            it->setTextAlignment(Qt::AlignCenter);

            setItem(row, col, it);
        }
        // Total/average marks
        item(row, lastCol)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
}

void TableStandings::resizeEvent(QResizeEvent *event)
{
    horizontalHeader()->resizeSection(0, event->size().width() / 5);
}

void TableStandings::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePress";
    if (!itemAt(event->pos())) {
        setCurrentCell(0, 0, QItemSelectionModel::Clear);
        qDebug() << "Clear";
    } else {
        QTableWidget::mousePressEvent(event);
    }
}
