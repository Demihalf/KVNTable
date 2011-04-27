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

#ifndef TABLESTAGESTANDINGS_H
#define TABLESTAGESTANDINGS_H

#include "tablestandings.h"

class TableStageStandings : public TableStandings
{
    Q_OBJECT

public:
    TableStageStandings(const QStringList &teams, int numOfMarks, int stageNumber,
                   QWidget *parent = 0);
    virtual ~TableStageStandings();

    QList<double> averages();
    double averageAt(int row);

    void setIntermediateResults(const QList<double> &mark);

public slots:
    void recalculateAverage();

signals:
    void marksChanged();

protected:
    virtual void createHeader();

    // Redefinition
    virtual void createLastCells();

private:
    int m_stageNumber;
};

#endif // TABLESTAGESTANDINGS_H
