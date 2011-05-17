/******************************************************************************
** <PROJECT_NAME> <PROJECT_VERSION>
** Copyright (C) 2011 Valery Kharitonov <kharvd@gmail.com>
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
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#ifndef TABLECONTAINER_H
#define TABLECONTAINER_H

#include <QObject>

// Just a convenience POD struct
struct Team {
    QString Name;
    QVector<QVector<int> > Grades; // [stage][judge]
    QVector<double> Penalties; // [stage]
    QVector<double> Averages; // [stage]
    double Total;
};

class TableContainer : public QObject
{
    Q_OBJECT
public:
    explicit TableContainer(QObject *parent = 0);

signals:

private:
    QVector<Team> m_teams;
};

#endif // TABLECONTAINER_H
