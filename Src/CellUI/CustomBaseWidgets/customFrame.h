// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QFrame>
#include "CellWidgetGlobalInterface.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

class customFrame : public QFrame, implements CellWidgetGlobalInterface
{
friend class customOptionBlock;
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    enum class Type{
        Regular, Radius
    };
    explicit customFrame(Type type, QWidget *parent = nullptr);
    virtual ~customFrame() override = default;

protected:
    //! Inhrited from
    //! CellWidgetGlobalInterface.
    inline virtual void
    init() override {}

    inline virtual void
    setEventConnections() override {}

    virtual void
    setColor(const QColor &color) override;

    virtual void
    changeToColor(const QColor& startColor, const QColor &targetColor, int duration) override;
    //! Inhrited from
    //! QFrame.
    virtual void
    paintEvent(QPaintEvent *) override;

private:
    Type mType;

public Q_SLOTS:
    inline virtual void
    setColorScheme(Cell::ColorScheme mode) override { CellWidgetGlobalInterface::setColorScheme(mode); }
};
#endif
