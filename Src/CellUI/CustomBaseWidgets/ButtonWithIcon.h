// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#ifndef BUTTONWITHICON_H
#define BUTTONWITHICON_H

#include "customButton.h"

class QLabel;
class QHBoxLayout;

class ButtonWithIcon : public customButton{
    Q_OBJECT
public:
    explicit ButtonWithIcon(customButton::Type type = Static,QWidget *parent = nullptr);

    void init(const QString& fileName, int iconWidth, int iconHeight);

protected:
    QHBoxLayout *mainLayout;
    QLabel *icon;
};

#endif // BUTTONWITHICON_H
