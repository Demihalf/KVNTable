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

StageStandingsModel::StageStandingsModel(TableContainer *container, int in_stageNum,
                                         QObject *parent) :
    QAbstractTableModel(parent), m_stageNum(in_stageNum), m_container(container)
{
    Q_ASSERT(m_container);

    m_judgesCount = m_container->judgesCount();

    m_headerLabels << tr("Team");
    for (int i = 1; i <= m_judgesCount; i++) {
        m_headerLabels << QString("%1").arg(i);
    }
    m_headerLabels << tr("Penalty") << tr("Average");

    if (m_stageNum != 0) {
        m_headerLabels << tr("Total");
    }

    connectContainerSignals();
}

int StageStandingsModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_container->teamsCount();
}

int StageStandingsModel::columnCount(const QModelIndex & /*parent*/) const
{
    // team + judges' marks + penalty + average + total (if not the first stage)
    return m_judgesCount + 3 + (m_stageNum != 1 ? 1 : 0);
}

QVariant StageStandingsModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_headerLabels.at(section);
    }

    return QVariant();
}

QVariant StageStandingsModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        int col = index.column();
        int row = index.row();

        // Team name
        if (col == 0) {
            return m_container->teamName(row);
        }

        // Marks
        if (col >= 1 && col <= m_judgesCount) {
            int grade = m_container->grade(row, m_stageNum, col - 1);
            return (grade == -1 ? "" : QString("%1").arg(grade));
        }

        // Penalty
        if (col == m_judgesCount + 1) {
            double penalty = m_container->penalty(row, m_stageNum);
            return (penalty == -1 ? "" : QString("%1").arg(penalty));
        }

        // Average
        if (col == m_judgesCount + 2) {
            double average = m_container->average(row, m_stageNum);
            return (average == -1 ? "" : QString("%1").arg(average));
        }

        // Total
        if (m_stageNum != 0 && col == m_judgesCount + 3) {
            double total = m_container->total(row, m_stageNum);
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

        int col = index.column();
        int row = index.row();

        disconnectContainerSignals();

        // Team name
        if (col == 0) {
            m_container->setTeamName(row, value.toString());
        }

        // Marks
        if (col >= 1 && col <= m_judgesCount) {
            m_container->setGrade(row, m_stageNum, col - 1,
                                  value.toString().toInt());
        }

        // Penalty
        if (col == m_judgesCount + 1) {
            m_container->setPenalty(row, m_stageNum, value.toString().toDouble());
        }

        connectContainerSignals();

        // Average and total (not editable)
        if (col == m_judgesCount + 2 ||
            (m_stageNum != 0 && col == m_judgesCount + 3))
        {
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

void StageStandingsModel::teamAboutToBeInserted(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void StageStandingsModel::teamInserted(int index)
{
    endInsertRows();
}

void StageStandingsModel::teamAboutToBeRemoved(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void StageStandingsModel::teamRemoved(int index)
{
    endRemoveRows();
}

void StageStandingsModel::teamNameChanged(int i)
{
    emit dataChanged(index(i, 0), index(i, 0));
}

void StageStandingsModel::gradeChanged(int team, int stage, int judge)
{
    if (stage == m_stageNum) {
        emit dataChanged(index(team, judge + 1), index(team, judge + 1));
    }
}

void StageStandingsModel::penaltyChanged(int team, int stage)
{
    if (stage == m_stageNum) {
        emit dataChanged(index(team, m_judgesCount + 1),
                         index(team, m_judgesCount + 1));
    }
}

void StageStandingsModel::averagesChanged(int stage)
{
    if (stage == m_stageNum) {
        emit dataChanged(index(0, m_judgesCount + 2),
                         index(rowCount(), m_judgesCount + 2));
    }
}

void StageStandingsModel::totalsChanged()
{
    if (stage != 0) {
        emit dataChanged(index(0, m_judgesCount + 3),
                         index(rowCount(), m_judgesCount + 3));
    }
}

void StageStandingsModel::connectContainerSignals()
{
    connect(m_container, SIGNAL(teamAboutToBeInserted(int)),
            SLOT(teamAboutToBeInserted(int)));
    connect(m_container, SIGNAL(teamInserted(int)), SLOT(teamInserted(int)));
    connect(m_container, SIGNAL(teamAboutToBeRemoved(int)),
            SLOT(teamAboutToBeRemoved(int)));
    connect(m_container, SIGNAL(teamRemoved(int)), SLOT(teamRemoved(int)));
    connect(m_container, SIGNAL(teamNameChanged(int)),
            SLOT(teamNameChanged(int)));
    connect(m_container, SIGNAL(gradeChanged(int,int,int)),
            SLOT(gradeChanged(int,int,int)));
    connect(m_container, SIGNAL(penaltyChanged(int,int)),
            SLOT(penaltyChanged(int,int)));
    connect(m_container, SIGNAL(averagesChanged(int)), SLOT(averagesChanged(int)));
    connect(m_container, SIGNAL(totalsChanged()), SLOT(totalsChanged()));
}

void StageStandingsModel::disconnectContainerSignals()
{
    disconnect(this, SLOT(teamAboutToBeInserted(int)));
    disconnect(this, SLOT(teamInserted(int)));
    disconnect(this, SLOT(teamAboutToBeRemoved(int)));
    disconnect(this, SLOT(teamRemoved(int)));
    disconnect(this, SLOT(teamNameChanged(int)));
    disconnect(this, SLOT(gradeChanged(int,int,int)));
    disconnect(this, SLOT(penaltyChanged(int,int)));
    disconnect(this, SLOT(averagesChanged(int)));
    disconnect(this, SLOT(totalsChanged()));
}

