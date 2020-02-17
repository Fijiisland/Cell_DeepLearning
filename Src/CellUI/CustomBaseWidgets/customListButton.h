// Copyright 2018-2020 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
#ifndef CUSTOMLISTBUTTON_H
#define CUSTOMLISTBUTTON_H

#include "customButton.h"

class QHBoxLayout;
class customLabel;

class customListButton : public customButton{
friend class customButtonListWidget;
public:
    explicit customListButton(QWidget *parent = nullptr, const QString &text = "text");

    void   setMargins(int left = 0, int top = 0, int right = 0, int buttom = 0);
    void   setText(QString &text);

private:
    QHBoxLayout *mainLayout;
    customLabel *tag;

protected:
    void         init();
    virtual void setFont(const QFont &font);

protected Q_SLOTS:
    virtual void setColorScheme(CellUiGlobal::COLOR_SCHEME mode) override;
};

#endif // CUSTOMLISTBUTTON_H
