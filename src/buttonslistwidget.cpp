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

#include <QtDebug>

#include "buttonslistwidget.h"
#include "ui_buttonslistwidget.h"

ButtonsListWidget::ButtonsListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonsListWidget)
{
    ui->setupUi(this);
    connect(ui->btnAdd, SIGNAL(clicked()), SLOT(addEmptyItem()));
    connect(ui->btnRemove, SIGNAL(clicked()), SLOT(removeSelected()));
    connect(ui->btnUp, SIGNAL(clicked()), SLOT(moveUp()));
    connect(ui->btnDown, SIGNAL(clicked()), SLOT(moveDown()));
    connect(ui->listBox, SIGNAL(itemChanged(QListWidgetItem*)), SIGNAL(changed()));
}

void ButtonsListWidget::addEmptyItem()
{
    ui->listBox->addItem("");
    QListWidgetItem *item = ui->listBox->item(ui->listBox->count() - 1);
    Q_ASSERT(item);
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->listBox->setCurrentItem(item);
    ui->listBox->editItem(item);
    emit changed();
}

void ButtonsListWidget::removeSelected()
{
    ui->listBox->takeItem(ui->listBox->currentRow());
    emit changed();
}

void ButtonsListWidget::moveUp()
{
    int currentRow = ui->listBox->currentRow();
    qDebug() << currentRow;
    if (currentRow > 0) {
        QListWidgetItem *tmp = ui->listBox->takeItem(ui->listBox->currentRow());
        ui->listBox->insertItem(currentRow - 1, tmp);
        ui->listBox->setCurrentItem(tmp);
    }

    emit changed();
}

void ButtonsListWidget::moveDown()
{
    int currentRow = ui->listBox->currentRow();
    qDebug() << currentRow;
    if (currentRow < ui->listBox->count() - 1) {
        QListWidgetItem *tmp = ui->listBox->takeItem(ui->listBox->currentRow());
        ui->listBox->insertItem(currentRow + 1, tmp);
        ui->listBox->setCurrentItem(tmp);
    }

    emit changed();
}

QStringList ButtonsListWidget::items()
{
    QStringList tmp;
    for (int i = 0; i < ui->listBox->count(); i++) {
        tmp << ui->listBox->item(i)->text();
    }

    return tmp;
}

void ButtonsListWidget::clear()
{
    ui->listBox->clear();
}

ButtonsListWidget::~ButtonsListWidget()
{
    delete ui;
}
