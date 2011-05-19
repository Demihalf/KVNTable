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
#include <QStringList>

class Team;

class TableContainer : public QObject
{
    Q_OBJECT
public:
    TableContainer(int judgesCount = 5, QObject *parent = 0);
    ~TableContainer();

    int teamsCount();
    int stagesCount();
    int judgesCount();

    void addTeam();
    void removeTeam(int i);
    Team* takeTeam(int i);
    void insertTeam(int i, Team *team = 0);

    void addStage();
    void removeStage(int i);
    void insertStage(int i, const QVector<Team *> &stage);
    QVector<Team *> takeStage(int i);

    QString teamName(int team);
    void setTeamName(int team, const QString &in_name);

    int grade(int team, int stage, int judge);
    void setGrade(int team, int stage, int judge, int in_grade);

    double penalty(int team, int stage);
    void setPenalty(int team, int stage, double in_penalty);

    double average(int team, int stage);

    double total(int team, int stage);
    double overallResult(int team);

signals:
    void teamAboutToBeInserted(int index);
    void teamInserted(int index);

    void teamAboutToBeRemoved(int index);
    void teamRemoved(int index);

    void stageAboutToBeInserted(int index);
    void stageInserted(int index);

    void stageAboutToBeRemoved(int index);
    void stageRemoved(int index);

    void teamNameChanged(int i);
    void gradeChanged(int team, int stage, int judge);
    void penaltyChanged(int team, int stage);
    void averagesChanged(int stage);
    void totalsChanged();
    void overallChanged();

private:
    QList<Team *> m_teams;
    int m_stagesCount;
    int m_judgesCount;
};

#endif // TABLECONTAINER_H
