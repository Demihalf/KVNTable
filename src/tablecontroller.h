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

#ifndef TABLECONTROLLER_H
#define TABLECONTROLLER_H

#include <QObject>
#include "tablecontainer.h"

class StageStandingsModel;
class TotalStandingsModel;

class TableController : public QObject
{
    Q_OBJECT

public:
    TableController(int in_judgesCount = 5, QObject *parent = 0);

    void addTeam(const QString &name = QString());
    void removeTeam(int i);
    void moveTeamUp(int team);
    void moveTeamDown(int team);

    StageStandingsModel * addStage(const QString &name = QString());
    StageStandingsModel * stageAt(int i);
    void moveStage(int from, int to);
    void removeStage(int i);

    TotalStandingsModel * totalModel();

    void clearAll();

private:
    TableContainer *m_container;
    QList<StageStandingsModel *> m_stageModels;
    TotalStandingsModel m_totalModel;
};

#endif // TABLECONTROLLER_H
