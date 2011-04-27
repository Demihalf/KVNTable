/******************************************************************************
** KVNTable 0.2
** Copyright (C) 2011 Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of KVNTable.
**
** $QT_BEGIN_LICENSE:GPL$
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
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "newtabledialog.h"
#include "ui_newtabledialog.h"
#include <QMessageBox>

NewTableDialog::NewTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTableDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(okClicked()));
}

void NewTableDialog::okClicked()
{
    if (teams().size() == 0 || stages().size() == 0) {
        QMessageBox::critical(this, "Ошибка", "Вы должны добавить хотя "
                              "бы одну команду и один конкурс!");
    } else {
        accept();
    }
}

QStringList NewTableDialog::teams()
{
    return ui->listTeams->items();
}

QStringList NewTableDialog::stages()
{
    return ui->listStages->items();
}

NewTableDialog::~NewTableDialog()
{
    delete ui;
}
