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

#ifndef TABLESTANDINGS_H
#define TABLESTANDINGS_H

#include <QTableWidget>

class TableStandings : public QTableWidget
{
    Q_OBJECT

public:
    TableStandings(const QStringList &teams, int middleCells,
                            QWidget *parent = 0);

    virtual ~TableStandings() {}

    void resizeTeamSection(int newSize);

signals:
    void teamSectionWidthChanged(int newSize);

private slots:
    void sectionWidthChanged(int index, int, int newSize);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

    virtual void createHeader() = 0;
    virtual void createTeams();
    virtual void createCells();

    QStringList m_teams;
    int m_numOfMiddleCells;
};

#endif // TABLESTANDINGS_H
