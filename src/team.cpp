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

#include "team.h"

Team::Team(int stagesCount, int judgesCount, QObject *parent) :
    QObject(parent), m_judgesCount(judgesCount), m_stagesCount(stagesCount)
{
    m_grades.resize(m_stagesCount);

    for (int i = 0; i < m_stagesCount; i++) {
        m_grades[i].fill(-1, m_judgesCount);
    }

    m_penalties.fill(-1, m_stagesCount);
    m_averages.fill(-1, m_stagesCount);
    m_totals.fill(-1, m_stagesCount);
}

QString Team::name() const
{
    return m_name;
}

void Team::setName(const QString &in_name)
{
    m_name = in_name;
    emit nameChanged();
}


int Team::grade(int stage, int judge) const
{
    if ((stage >= 0) && (stage < m_stagesCount)
            && (judge >= 0) && (judge < m_judgesCount))
    {
        return m_grades.at(stage).at(judge);
    }

    return -1;
}

void Team::setGrade(int stage, int judge, int in_grade)
{
    if ((stage >= 0) && (stage < m_stagesCount)
            && (judge >= 0) && (judge < m_judgesCount))
    {
        m_grades[stage][judge] = in_grade;
        refreshAverage(stage);
        emit gradeChanged(stage, judge);
    }
}

double Team::penalty(int stage) const
{
    if ((stage >= 0) && (stage < m_stagesCount))
    {
        return m_penalties.at(stage);
    }

    return -1;
}

void Team::setPenalty(int stage, double in_penalty)
{
    if ((stage >= 0) && (stage < m_stagesCount))
    {
        m_penalties[stage] = in_penalty;
        refreshAverage(stage);
        emit penaltyChanged(stage);
    }
}

double Team::average(int stage) const
{
    if ((stage >= 0) && (stage < m_stagesCount))
    {
        return m_averages.at(stage);
    }

    return -1;
}

double Team::total(int stage) const
{
    if ((stage >= 0) && (stage < m_stagesCount))
    {
        return m_totals.at(stage);
    }

    return -1;
}

double Team::overallResult() const
{
    if (m_stagesCount > 0) {
        return m_totals.at(m_stagesCount - 1);
    }

    return -1;
}

void Team::addStage()
{
    m_stagesCount++;
    m_grades.append(QVector<int>(m_judgesCount, -1));
    m_penalties.append(-1);
    m_averages.append(-1);
    m_totals.append(-1);

    refreshTotals();

    emit stageInserted(m_judgesCount - 1);
}

void Team::removeStage(int index)
{
    m_grades.remove(index);
    m_penalties.remove(index);
    m_averages.remove(index);
    m_totals.remove(index);

    m_stagesCount--;
    refreshTotals();

    emit stageRemoved(index);
}

Team* Team::takeStage(int index)
{
    Team *tmp = new Team(1, m_judgesCount);
    tmp->m_grades.replace(0, m_grades.at(index));
    tmp->m_penalties.replace(0, m_penalties.at(index));
    removeStage(index);

    return tmp;
}

void Team::insertStage(int index, Team *stage)
{
    m_grades.insert(index, stage->m_grades.at(0));
    m_penalties.insert(index, stage->m_penalties.at(0));
    m_averages.insert(index, -1);
    m_totals.insert(index, -1);

    delete stage;

    refreshAverage(index);
    refreshTotals();

    emit stageInserted(index);
}

void Team::refreshAverage(int stage)
{
    double sum = 0;

    for (int j = 0; j < m_judgesCount; j++) {
        sum += m_grades.at(stage).at(j);
    }

    sum /= m_judgesCount;
    sum -= m_penalties.at(stage);

    m_averages.replace(stage, sum);

    emit averageChanged(stage);
}

void Team::refreshTotals()
{
    m_totals.replace(0, m_averages.at(0));

    for (int i = 1; i < m_stagesCount; i++) {
        double sum = m_totals.at(i - 1) + m_averages.at(i);
        m_totals.replace(i, sum);
    }

    emit totalsChanged();
    emit overallChanged();
}
