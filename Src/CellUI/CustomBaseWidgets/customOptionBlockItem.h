// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
#ifndef CUSTOMOPTIONBLOCKITEM_H
#define CUSTOMOPTIONBLOCKITEM_H

#include <QWidget>
#include "CellWidgetGlobalInterface.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

class QHBoxLayout;
class customLabel;

class customOptionBlockItem : public QWidget, implements CellWidgetGlobalInterface{
friend class customOptionBlock;
    Q_OBJECT
public:
enum MarginType{
    Left, Right, Top, Bottom
};
static int TagTextSize;
    explicit customOptionBlockItem(QWidget *parent = nullptr, const QString& tag = " ");

    inline int
    getTagLen() const { return tagLen; }

    void
    setTag(const QString& text);

    void
    setOptionWidget(QWidget* widget);

    void
    setHint(const QString& text);

    void
    setMargins(int left, int top, int right, int bottom);

    void
    setMargin(MarginType direction, int margin);

    void
    setSpacing(int value);

    const QMargins
    getMargins() const;

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
    changeToColor(const QColor &startColor, const QColor &targetColor, int duration) override;

    QHBoxLayout *mainLayout;
    customLabel *tag;
    QWidget     *optionWidget;
    customLabel *hint;

    int itemHeight = 0;
    int tagLen     = 0;

public Q_SLOTS:
    virtual void
    setColorScheme(Cell::ColorScheme mode) override;
};

#endif // CUSTOMOPTIONBLOCKITEM_H
