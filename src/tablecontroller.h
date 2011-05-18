///******************************************************************************
//** <PROJECT_NAME> <PROJECT_VERSION>
//** Copyright (C) 2011 Valery Kharitonov <kharvd@gmail.com>
//**
//** This program is free software: you can redistribute it and/or modify
//** it under the terms of the GNU General Public License as published by
//** the Free Software Foundation, either version 3 of the License, or
//** (at your option) any later version.
//**
//** This program is distributed in the hope that it will be useful,
//** but WITHOUT ANY WARRANTY; without even the implied warranty of
//** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//** GNU General Public License for more details.
//**
//** You should have received a copy of the GNU General Public License
//** along with this program.  If not, see <http://www.gnu.org/licenses/>.
//**
//** If you have questions regarding the use of this file, please contact me at
//** kharvd@gmail.com.
//**
//******************************************************************************/

//#ifndef TABLECONTROLLER_H
//#define TABLECONTROLLER_H

//#include <QObject>

//class StageStandingsModel;
//class TotalStandingsModel;
//class TableContainer;
//struct Team;

//class TableController : public QObject
//{
//    Q_OBJECT

//public:
//    TableController(const QStringList &in_stages, int in_judgesCount,
//                    QObject *parent = 0);

//    void addTeam(const QString &name = QString());
//    StageStandingsModel * addStage(const QString &name = QString());
//    StageStandingsModel * stageAt(int i);
//    TotalStandingsModel * totalModel();
//    void removeTeam(int i);
//    void removeStage(int i);
//    void insertTeam(int i, Team *team);
//    void insertStage(int i, StageStandingsModel *stage);
//    Team takeTeamAt(int i);
//    StageStandingsModel * takeStageAt(int i);
//    void clearAll();

//private slots:

//private:
//    TableContainer *m_container;
//    QList<StageStandingsModel *> m_stageModels;
////    TotalStandingsModel m_totalModel;
//};

//#endif // TABLECONTROLLER_H
