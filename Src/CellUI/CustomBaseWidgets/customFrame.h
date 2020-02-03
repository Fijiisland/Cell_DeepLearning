// Copyright 2018-2019 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
//
#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QFrame>
#include "CellWidgetGlobalInterface.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

class customFrame : public QFrame, implements CellWidgetGlobalInterface
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    explicit customFrame(QWidget *parent = nullptr);
    virtual ~customFrame() override = default;

    virtual void setColor(const QColor &color) override;

protected:
    virtual void setBaseQss(const QString& qss) override;
    virtual void changeToColor(const QColor& startColor, const QColor &targetColor) override;

public slots:
    virtual void setColorScheme(CellUiGlobal::COLOR_SCHEME mode) override;
};
#endif
