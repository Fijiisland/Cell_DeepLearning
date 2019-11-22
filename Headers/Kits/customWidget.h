// Copyright 2019 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
//
// This class implemented dropshadow effect & "color" custom property & qss override
#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include "cell_util.h" // WIDGET_TYPE

class customWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit customWidget(WIDGET_TYPE type, QWidget *parent = nullptr);

public:
    const QColor color() const;
    void setColor(const QColor &color);

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
    virtual void paintEvent(QPaintEvent *event) override;
    void LoadWinStyle(QWidget *obj);

protected:
    QColor m_color;
    WIDGET_TYPE m_type;
};
#endif
