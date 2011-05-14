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

#include "stagestandingsmodel.h"

StageStandingsModel::StageStandingsModel(int in_judgesCount, int in_stageNum,
                                         QObject *parent) :
    QAbstractTableModel(parent)
{
    m_judgesCount = in_judgesCount;
    m_stageNum = in_stageNum;

    m_headerLabels << tr("Team");
    for (int i = 1; i <= m_judgesCount; i++) {
        m_headerLabels << QString("%1").arg(i);
    }
    m_headerLabels << tr("Penalty") << tr("Average");

    if (m_stageNum != 0) {
        m_headerLabels << tr("Total");
    }

    connect(this, SIGNAL(marksChanged(int)), SLOT(updateAverage(int)));
}

int StageStandingsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_teams.size();
}

int StageStandingsModel::columnCount(const QModelIndex & /*parent*/) const
{
    // team + judges' marks + penalty + average + total (if not the first stage)
    return m_judgesCount + 3 + (m_stageNum != 1 ? 1 : 0);
}

bool StageStandingsModel::insertRows(int row, int count,
                                     const QModelIndex & /*parent*/)
{
    if (row > rowCount()) {
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++) {
        Team tmp;
        tmp.Name = "";
        tmp.Average = -1;
        tmp.Penalty = -1;
        tmp.Total = -1;
        tmp.Grades.resize(m_judgesCount);

        for (int j = 0; j < tmp.Grades.size(); j++) {
            tmp.Grades.replace(j, -1);
        }

        m_teams.insert(row + i, tmp);
    }

    endInsertRows();

    return true;
}

bool StageStandingsModel::removeRows(int row, int count,
                                     const QModelIndex & /*parent*/)
{
    if (row > rowCount() - count) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++) {
        m_teams.removeAt(row + i);
    }

    endRemoveRows();

    return true;
}

QVariant StageStandingsModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_headerLabels.at(section);
    } else {
        return QVariant();
    }
}

QVariant StageStandingsModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        Team tmp = m_teams.at(index.row());

        int col = index.column();

        // Team name
        if (col == 0) {
            return tmp.Name;
        }

        // Marks
        if (col >= 1 && col <= m_judgesCount) {
            int grade = tmp.Grades.at(col - 1);
            return (grade == -1 ? "" : QString("%1").arg(grade));
        }

        // Penalty
        if (col == m_judgesCount + 1) {
            double penalty = tmp.Penalty;
            return (penalty == -1 ? "" : QString("%1").arg(penalty));
        }

        // Average
        if (col == m_judgesCount + 2) {
            double average = tmp.Average;
            return (average == -1 ? "" : QString("%1").arg(average));
        }

        // Total
        if (m_stageNum != 0 && col == m_judgesCount + 3) {
            double total = tmp.Total;
            return (total == -1 ? "" : QString("%1").arg(total));
        }

        return QVariant();
    }

    return QVariant();
}

bool StageStandingsModel::setData(const QModelIndex & index, const QVariant & value,
             int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.row() >= rowCount() || index.column() >= columnCount()) {
            return false;
        }

        Team tmp = m_teams.at(index.row());

        int col = index.column();
        int row = index.row();

        // Team name
        if (col == 0) {
            tmp.Name = value.toString();
            m_teams.replace(row, tmp);
            emit teamNameChanged(row, tmp.Name);
        }

        // Marks
        if (col >= 1 && col <= m_judgesCount) {
            tmp.Grades.replace(col - 1, value.toString().toInt());
            emit marksChanged(row);
        }

        // Penalty
        if (col == m_judgesCount + 1) {
            tmp.Penalty = value.toString().toDouble();
            emit marksChanged(row);
        }

        // Average
        if (col == m_judgesCount + 2) {
            return false;
        }

        // Total
        if (m_stageNum != 0 && col == m_judgesCount + 3) {
            return false;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags StageStandingsModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    int col = index.column();

    // Team name, marks and penalty
    if (col >= 0 && col <= m_judgesCount + 2) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    // Average and total
    if ((col == m_judgesCount + 3)
            || (m_stageNum != 0 && col == m_judgesCount + 4)) {
        return QAbstractTableModel::flags(index) & !Qt::ItemIsEditable;
    }

    return QAbstractTableModel::flags(index);
}

void StageStandingsModel::updateAverage(int team)
{
    Team tmp = m_teams.at(team);

    double sum = 0;
    for (int j = 0; j < m_judgesCount; j++) {
        sum += tmp.Grades.at(j);
    }
    sum -= tmp.Penalty;

    m_teams.replace(team, tmp);

    // Averages have been changed
    emit dataChanged(this->index(team, m_judgesCount + 2),
                     this->index(team, m_judgesCount + 2));
    emit averageChanged(team);
}

double StageStandingsModel::averageAt(int index)
{
    return m_teams.at(index).Average;
}

void StageStandingsModel::setTotalAt(int index, double total)
{
    Team tmp = m_teams.at(index);
    tmp.Total = total;

    m_teams.replace(index, tmp);

    emit dataChanged(this->index(index, m_judgesCount + 3),
                     this->index(index, m_judgesCount + 3));
}

void StageStandingsModel::setTeamNameAt(int index, const QString &name)
{
    Team tmp = m_teams.at(index);
    tmp.Name = name;

    m_teams.replace(index, tmp);

    emit dataChanged(this->index(index, 0), this->index(index, 0));
}
