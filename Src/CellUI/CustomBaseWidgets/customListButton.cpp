// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#include "../../CellCore/CellNamespace.h"
#include "../../CellCore/Kits/CellUtility.h"
#include "../CustomBaseWidgets/customListButton.h"
#include "../CustomBaseWidgets/customLabel.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>

customListButton::customListButton(QWidget *parent, const QString &text) :
    customButton(customButton::Checkable, parent)
  , mainLayout(new QHBoxLayout(this))
  , tag(new customLabel(this))
{
    tag->setText(text);
    init();
}

void customListButton::init()
{
    setLayout(mainLayout);

    mainLayout->addWidget(tag);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(12, 0, 0, 0);

    tag->setBrightDarkColor(Cell::CGL70, Cell::CGL255);
    CellUiGlobal::setCustomTextLabel(tag, CHAR2STR("Microsoft YaHei UI Light"), 16);

    CellWidgetGlobalInterface::_modules << tag;
}

void customListButton::setFont(const QFont &font){
    tag->setFont(font);
}

void customListButton::setMargins(int left, int top, int right, int buttom){
    mainLayout->setContentsMargins(left, top, right, buttom);
}

void customListButton::setText(QString &text){
    tag->setText(text);
}
