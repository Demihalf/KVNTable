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

#include "tablestandings.h"
#include <QHeaderView>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>

TableStandings::TableStandings(const QStringList &teams, int middleCells,
                               QWidget *parent) :
    QTableWidget(parent), m_teams(teams), m_numOfMiddleCells(middleCells)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->hide();

    setAlternatingRowColors(true);

    connect(horizontalHeader(), SIGNAL(sectionResized(int,int,int)),
            SLOT(sectionWidthChanged(int,int,int)));

    setRowCount(teams.size());

    horizontalHeader()->resizeSection(0, size().width() / 5);
}

TableData TableStandings::getData()
{
    TableData data;

    data.resize(rowCount());

    for (int i = 0; i < rowCount(); i++) {
        data[i].resize(columnCount());
        for (int j = 0; j < columnCount(); j++) {
            data[i][j] = *item(i, j);
        }
    }

    return data;
}

void TableStandings::setData(const TableData &data)
{
    disconnectSignals();

    clearContents();

    setRowCount(data.size());
    setColumnCount(data[0].size());

    for (int i = 0; i < rowCount(); i++) {
        for (int j = 0; j < columnCount(); j++) {
            QTableWidgetItem *it = new QTableWidgetItem(data[i][j]);
            setItem(i, j, it);
        }
    }

    reconnectSignals();
}

void TableStandings::createTeams()
{
    int row = 0;

    foreach (QString team, m_teams) {
        QTableWidgetItem *it = new QTableWidgetItem(team);

        it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        it->setTextAlignment(Qt::AlignCenter);

        setItem(row, 0, it);

        row++;
    }
}

void TableStandings::resizeTeamSection(int newSize)
{
    horizontalHeader()->resizeSection(0, newSize);
}

void TableStandings::sectionWidthChanged(int index, int, int newSize)
{
    if (index == 0) {
        emit teamSectionWidthChanged(newSize);
    }
}

void TableStandings::createCells()
{
    createTeams();
    createMiddleCells();
    createLastCells();
}

void TableStandings::createMiddleCells()
{
    for (int row = 0; row < m_teams.size(); row++) {
        for (int col = 1; col <= m_numOfMiddleCells; col++) {
            QTableWidgetItem * it = new QTableWidgetItem();

            it->setTextAlignment(Qt::AlignCenter);

            setItem(row, col, it);
        }
    }
}

void TableStandings::mousePressEvent(QMouseEvent *event)
{
    if (!itemAt(event->pos())) {
        setCurrentCell(0, 0, QItemSelectionModel::Clear);
    } else {
        QTableWidget::mousePressEvent(event);
    }
}
