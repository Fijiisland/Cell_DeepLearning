// Copyright 2019 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
#ifndef CELL_UTIL_H
#define CELL_UTIL_H

#include <QMetaType> // Q_DECLARE_METATYPE()

class QGraphicsDropShadowEffect;
class QPropertyAnimation;
class QStyleSheetLoader;
class QGraphicsEffect;

namespace CELL_UTIL{
namespace TOOLS{
extern void setDropShadowEffect(QList<QGraphicsDropShadowEffect*> animis,
                                QList<QWidget*> modules,
                                QPoint offset,
                                QColor color, int blurRadius);
extern void setPropertyAnimation(QList<QPropertyAnimation*> animis,
                                 QByteArray _property,
                                 QVariant s_value,
                                 QVariant e_value, int duration,
                                 QEasingCurve curve,
                                 QList<QWidget*> modules,
                                 QGraphicsEffect *effect = nullptr);
extern void multiModulesOneStyleSheet(QList<QWidget*> modules, QString qss);
extern QStyleSheetLoader *styleSheetLoader;
} // namespace TOOLS{
namespace LITERAL{
extern const char* const  INSTANCE_IS_ON;
extern const QString      IMG_DIR;
extern const QString      FONT_DIR;
extern const QString      STYLESHEET_DIR;
extern const QString      LIB_DIR;
extern const QString      COLOR_SPACE_GRAY;
extern const QColor       COLOR_OPTION_BLOCK_DARK;
extern const QColor       COLOR_OPTION_BLOCK_BRIGHT;
extern const QColor       MAINWINDOW_DARK;
extern const QColor       MAINWINDOW_BRIGHT;
} // namespace LITERAL{
} // namespace UTILITY{

enum FADE_TYPE{_IN, _OUT};
enum PAGE_TYPE{_HOME, _SETTINGS};
enum COLOR_SCHEME{_BRIGHT, _DARK};
enum FRAME_TYPE{_REGULAR, _OPTIONBLOCK};
enum WIDGET_TYPE{_BASE, _CHILD};
Q_DECLARE_METATYPE(COLOR_SCHEME);

#endif

