// Copyright 2018-2020 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
#include "ButtonWithIcon.h"

#include <QHBoxLayout>
#include <QLabel>

ButtonWithIcon::ButtonWithIcon(customButton::TYPE type, QWidget *parent):
    customButton(type, parent),
    mainLayout(new QHBoxLayout(this)),
    icon(new QLabel(this))
{
    setLayout(mainLayout);
    setCursor(Qt::PointingHandCursor);
    
    mainLayout->setMargin(0);
    mainLayout->addWidget(icon);
    mainLayout->setAlignment(Qt::AlignmentFlag::AlignHCenter);
}

void ButtonWithIcon::Init(const QString& fileName, int iconWidth, int iconHeight)
{
    CellUiGlobal::setLabelPixmap(icon, fileName, iconWidth, iconHeight);
}
