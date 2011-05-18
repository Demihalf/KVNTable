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

#include "tablecontainer.h"

#include <QVector>
#include <team.h>

TableContainer::TableContainer(int judgesCount, QObject *parent) :
    QObject(parent), m_judgesCount(judgesCount)
{

}

TableContainer::~TableContainer()
{

}

int TableContainer::teamsCount()
{
    return m_teams.size();
}

int TableContainer::stagesCount()
{
    return m_stagesCount;
}

void TableContainer::addTeam()
{
    insertTeam(m_teams, 0);
}

void TableContainer::removeTeam(int i)
{
    if (i >= 0 && i < m_teams.size()) {
        emit teamAboutToBeRemoved(i);

        Team *tmp = m_teams.takeAt(i);
        delete tmp;

        emit teamRemoved(i);
    }
}

Team* TableContainer::takeTeam(int i)
{
    if (i >= 0 && i < m_teams.size()) {
        emit teamAboutToBeRemoved(i);

        Team *tmp = m_teams.at(i);
        m_teams.removeAt(i);
        tmp->setParent(NULL);

        emit teamRemoved(i);
        return tmp;
    }

    return NULL;
}

void TableContainer::insertTeam(int i, Team *team)
{
    if (i >= 0 && i <= m_teams.size()) {
        emit teamAboutToBeInserted(i);

        if (team == 0) {

            Team *tmp = new Team(m_stagesCount, m_judgesCount, this);
            m_teams.insert(i, tmp);

            connect(tmp, SIGNAL(averageChanged(int)), SIGNAL(averagesChanged(int)));
            connect(tmp, SIGNAL(totalsChanged()), SIGNAL(totalsChanged()));
            connect(tmp, SIGNAL(overallChanged()), SIGNAL(overallsChanged()));
        } else {
            m_teams.insert(i, team);
            team->setParent(this);
        }

        emit teamInserted(i);
    }
}

}

void TableContainer::addStage()
{
    emit stageAboutToBeInserted(m_stagesCount - 1);

    for (int i = 0; i < m_teams.size(); i++) {
        m_teams[i]->addStage();
    }

    emit stageInserted(m_stagesCount - 1);
}

void TableContainer::removeStage(int i)
{
    if (i >= 0 && i < m_stagesCount) {
        emit stageAboutToBeRemoved(i);

        for (int t = 0; t < m_teams.size(); t++) {
            m_teams[t]->removeStage(i);
        }

        emit stageRemoved(i);
    }
}

void TableContainer::insertStage(int i, const QVector<Team *> &stage)
{
    if (i >= 0 && i <= m_stagesCount) {
        emit stageAboutToBeInserted(i);

        for (int t = 0; t < m_teams.size(); t++) {
            m_teams[t]->insertStage(i, stage.at(t));
        }

        emit stageInserted(i);
    }
}

QVector<Team *> TableContainer::takeStage(int i)
{
    if (i >= 0 && i < m_stagesCount) {
        emit stageAboutToBeRemoved(i);

        QVector<Team *> tmp(m_teams.size());

        for (int t = 0; t < m_teams.size(); t++) {
            tmp.append(m_teams[t]->takeStage(i));
        }

        emit stageRemoved(i);
        return tmp;
    }

    return QVector<Team *>();
}

QString TableContainer::teamName(int team)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams.at(team)->name();
    }

    return QString();
}

void TableContainer::setTeamName(int team, const QString &in_name)
{
    if (team >= 0 && team < m_teams.size()) {
        m_teams[team]->setName(in_name);
        emit teamNameChanged(team);
    }
}

int TableContainer::grade(int team, int stage, int judge)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams[team]->grade(stage, judge);
    }

    return -1;
}

void TableContainer::setGrade(int team, int stage, int judge, int in_grade)
{
    if (team >= 0 && team < m_teams.size()) {
        m_teams[team]->setGrade(stage, judge, in_grade);
        emit gradeChanged(team, stage, judge);
    }
}

double TableContainer::penalty(int team, int stage)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams[team]->penalty(stage);
    }

    return -1;
}

void TableContainer::setPenalty(int team, int stage, double in_penalty)
{
    if (team >= 0 && team < m_teams.size()) {
        m_teams[team]->setPenalty(stage, in_penalty);
        emit penaltyChanged(team, stage);
    }
}

double TableContainer::average(int team, int stage)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams[team]->average(stage);
    }

    return -1;
}


double TableContainer::total(int team, int stage)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams[team]->total(stage);
    }

    return -1;
}

double TableContainer::overallResult(int team)
{
    if (team >= 0 && team < m_teams.size()) {
        return m_teams[team]->overallResult();
    }

    return -1;
}
