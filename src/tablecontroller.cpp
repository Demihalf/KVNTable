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

#include "tablecontroller.h"
#include "totalstandingsmodel.h"
#include "stagestandingsmodel.h"

TableController::TableController(int in_judgesCount, QObject *parent) :
    QObject(parent)
{
    m_container = new TableContainer(in_judgesCount, this);

    m_totalModel = new TotalStandingsModel(m_container, this);
}

void TableController::addTeam(const QString &name)
{
    int num = m_container->teamsCount();

    m_container->addTeam();
    m_container->setTeamName(num, name);
}

void TableController::removeTeam(int i)
{
    m_container->removeTeam(i);
}

void TableController::insertTeam(int i, Team *team)
{
    m_container->insertTeam(i, team);
}

Team* TableController::takeTeamAt(int i)
{
    return m_container->takeTeam(i);
}

StageStandingsModel * TableController::addStage(const QString &name)
{
    int num = m_container->stagesCount();

    m_container->addStage();

    StageStandingsModel *tmp = new StageStandingsModel(m_container, num, this);
    m_stageModels << tmp;

    return tmp;
}

StageStandingsModel * TableController::stageAt(int i)
{
    return m_stageModels.at(i);
}

void TableController::insertStage(int i, StageStandingsModel *stage)
{

}

StageStandingsModel * TableController::takeStageAt(int i)
{
    StageStandingsModel *tmp = m_stageModels.at(i);
    tmp->setStageNumber(-1);

}

void TableController::removeStage(int i)
{

}

TotalStandingsModel * TableController::totalModel()
{

}

void TableController::clearAll()
{

}

