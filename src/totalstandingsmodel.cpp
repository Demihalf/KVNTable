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

#include "totalstandingsmodel.h"

TotalStandingsModel::TotalStandingsModel(TableContainer *container, QObject *parent) :
    QAbstractTableModel(parent), m_container(container)
{
    Q_ASSERT(m_container);

    connectContainerSignals();
}

int TotalStandingsModel::rowCount(const QModelIndex & parent) const
{
    return m_container->teamsCount();
}

int TotalStandingsModel::columnCount(const QModelIndex & parent) const
{
    // Team + stages + total
    return m_container->stagesCount() + 2;
}

QVariant TotalStandingsModel::data(const QModelIndex & index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        int col = index.column();
        int row = index.row();

        // Team name
        if (col == 0) {
            return m_container->teamName(row);
        }

        // Marks
        if (col > 0 && col < columnCount() - 1) {
            int grade = m_container->average(row, col - 1);
            return (grade == -1 ? "" : QString("%1").arg(grade));
        }

        // Total
        if (col == columnCount() - 1) {
            double total = m_container->overallResult(row);
            return (total == -1 ? "" : QString("%1").arg(total));
        }

        return QVariant();
    }

    return QVariant();
}

QVariant TotalStandingsModel::headerData(int section, Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Team");
        }

        if (section == columnCount() - 1) {
            return tr("Total");
        }

        if (section > 0 && section < columnCount() - 1) {
            return QString("%1").arg(section);
        }

        return QVariant();
    }

    return QVariant();
}

bool TotalStandingsModel::setData(const QModelIndex & index, const QVariant & value,
                                  int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (index.row() >= rowCount() || index.column() >= columnCount()) {
            return false;
        }

        int col = index.column();
        int row = index.row();

        // Team name
        if (col == 0) {
            disconnectContainerSignals();

            m_container->setTeamName(row, value.toString());
            emit dataChanged(index, index);

            connectContainerSignals();

            return true;
        }

        return false;
    }

    return false;
}

Qt::ItemFlags TotalStandingsModel::flags(const QModelIndex & index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    int col = index.column();

    // Team name
    if (col == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    return QAbstractTableModel::flags(index) & !Qt::ItemIsEditable;
}

void TotalStandingsModel::teamAboutToBeInserted(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void TotalStandingsModel::teamInserted(int index)
{
    endInsertRows();
}

void TotalStandingsModel::teamAboutToBeRemoved(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void TotalStandingsModel::teamRemoved(int index)
{
    endRemoveRows();
}

void TotalStandingsModel::stageAboutToBeInserted(int index)
{
    beginInsertColumns(QModelIndex(), index + 1, index + 1);
}

void TotalStandingsModel::stageInserted(int index)
{
    endInsertColumns();
}

void TotalStandingsModel::stageAboutToBeRemoved(int index)
{
    beginRemoveColumns(QModelIndex(), index + 1, index + 1);
}

void TotalStandingsModel::stageRemoved(int index)
{
    endRemoveColumns();
}

void TotalStandingsModel::teamNameChanged(int i)
{
    emit dataChanged(index(i, 0), index(i, 0));
}

void TotalStandingsModel::averagesChanged(int stage)
{
    emit dataChanged(index(stage, 0), index(stage, rowCount()));
}

void TotalStandingsModel::connectContainerSignals()
{
    connect(m_container, SIGNAL(teamAboutToBeInserted(int)),
            SLOT(teamAboutToBeInserted(int)));
    connect(m_container, SIGNAL(teamInserted(int)), SLOT(teamInserted(int)));
    connect(m_container, SIGNAL(teamAboutToBeRemoved(int)),
            SLOT(teamAboutToBeRemoved(int)));
    connect(m_container, SIGNAL(teamRemoved(int)), SLOT(teamRemoved(int)));
    connect(m_container, SIGNAL(stageAboutToBeInserted(int)),
            SLOT(stageAboutToBeInserted(int)));
    connect(m_container, SIGNAL(stageInserted(int)), SLOT(stageInserted(int)));
    connect(m_container, SIGNAL(stageAboutToBeRemoved(int)),
            SLOT(stageAboutToBeRemoved(int)));
    connect(m_container, SIGNAL(stageRemoved(int)), SLOT(stageRemoved(int)));
    connect(m_container, SIGNAL(teamNameChanged(int)), SLOT(teamNameChanged(int)));
    connect(m_container, SIGNAL(averagesChanged(int)), SLOT(averagesChanged(int)));
}

void TotalStandingsModel::disconnectContainerSignals()
{
    disconnect(this, SLOT(teamAboutToBeInserted(int)));
    disconnect(this, SLOT(teamInserted(int)));
    disconnect(this, SLOT(teamAboutToBeRemoved(int)));
    disconnect(this, SLOT(teamRemoved(int)));
    disconnect(this, SLOT(stageAboutToBeInserted(int)));
    disconnect(this, SLOT(stageInserted(int)));
    disconnect(this, SLOT(stageAboutToBeRemoved(int)));
    disconnect(this, SLOT(stageRemoved(int)));
    disconnect(this, SLOT(teamNameChanged(int)));
    disconnect(this, SLOT(averagesChanged(int)));
}

