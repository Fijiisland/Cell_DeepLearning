// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#ifndef CELLWINSTYLEINTERFACE_H
#define CELLWINSTYLEINTERFACE_H

#include <QWidget>

#include "CellWidgetGlobalInterface.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

class customWinstyleWidget : public QWidget, implements CellWidgetGlobalInterface{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    explicit customWinstyleWidget(QWidget *parent = nullptr);
    virtual ~customWinstyleWidget() = default;

protected:
    //! Inheried from
    //! CellWidgetGlobalInterface.
    virtual void
    init() override;

    inline virtual void
    setEventConnections() override {}

    virtual void
    changeToColor(const QColor &startColor, const QColor &targetColor, int duration) override;

    virtual void
    setColor(const QColor &color) override;
    //! Inheried from
    //! QWidget.
    virtual bool
    nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    virtual void
    mousePressEvent(QMouseEvent *event) override;

    virtual void
    mouseMoveEvent(QMouseEvent *event) override;

    virtual void
    mouseReleaseEvent(QMouseEvent *event) override;

    void
    LoadWinStyle(QWidget *obj);

    QPoint m_startPoint;
    QPoint m_windowPoint;
    bool   m_move; 

public Q_SLOTS:
    virtual void
    setColorScheme(Cell::ColorScheme mode) override;
};
#endif // CELLWINSTYLEINTERFACE_H
