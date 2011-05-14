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

#ifndef STAGESTANDINGSMODEL_H
#define STAGESTANDINGSMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QStringList>

// Just a convenience POD struct
struct Team {
    QString Name;
    QVector<int> Grades;
    double Penalty;
    double Average;
    double Total;
};

/*
  [Team] [Judge 1] [Judge 2] ... [Judge N] [Penalty] [Average] [Total]
*/
class StageStandingsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StageStandingsModel(int in_judgesCount, int in_stageNum,
                        QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value,
                 int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const;

    bool insertRows(int row, int count,
                    const QModelIndex & parent = QModelIndex());
    bool removeRows(int row, int count,
                    const QModelIndex & parent = QModelIndex());

    double averageAt(int index);
    void setTotalAt(int index, double total);
    void setTeamNameAt(int index, const QString &name);

public slots:
    void updateAverage(int team);

signals:
    void teamNameChanged(int team, const QString &newName);

    // Including penalty
    void marksChanged(int team);
    void averageChanged(int team);

private:
    int m_stageNum;
    int m_judgesCount;

    QStringList m_headerLabels;
    QList<Team> m_teams;
};

#endif // STAGESTANDINGSMODEL_H
