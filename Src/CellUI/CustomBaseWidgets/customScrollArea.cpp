// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#include "customScrollArea.h"
#include "../../CellCore/Kits/CellUtility.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

customScrollArea::customScrollArea(QWidget *parent):
    QScrollArea(parent)
{
    setFrameShape(QFrame::Shape::NoFrame);
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

    CellUiGlobal::loader.setFileName(CHAR2STR("scrollBar.css"));
    setStyleSheet(CellUiGlobal::loader.content());
}
