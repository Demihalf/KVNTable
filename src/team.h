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

#ifndef TEAM_H
#define TEAM_H

#include <QObject>
#include <QVector>

// Keeps data up-to-date
class Team : public QObject {
private:
    Q_OBJECT

public:
    Team(int stagesCount = 0, int judgesCount = 5, QObject *parent = 0);
    ~Team() {}

    void addStage();
    void removeStage(int index);

    // Gets the slice of data with the corresponding grades
    Team* takeStage(int index);
    void insertStage(int index, Team *stage);

    QString name() const;
    void setName(const QString &in_name);

    int grade(int stage, int judge) const;
    void setGrade(int stage, int judge, int in_grade);

    double penalty(int stage) const;
    void setPenalty(int stage, double in_penalty);

    double average(int stage) const;

    double total(int stage) const;
    double overallResult() const;

signals:
    void stageInserted(int index);
    void stageRemoved(int index);

    void nameChanged();
    void gradeChanged(int stage, int judge);
    void penaltyChanged(int stage);
    void averageChanged(int stage);
    void totalsChanged();
    void overallChanged();

private:
    void refreshAverage(int stage);
    void refreshTotals();

    QString m_name;
    QVector<QVector<int> > m_grades; // [stage][judge]
    QVector<double> m_penalties; // [stage]
    QVector<double> m_averages; // [stage]
    QVector<double> m_totals; // [stage]

    int m_judgesCount;
    int m_stagesCount;
};

#endif // TEAM_H
