#ifndef CUSTOMSMOOTHSCROLLAREA_H
#define CUSTOMSMOOTHSCROLLAREA_H

#include <QScrollArea>

class customSmoothScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit customSmoothScrollArea(QWidget *parent = nullptr);
    ~customSmoothScrollArea() = default;
};

#endif // CUSTOMSMOOTHSCROLLAREA_H
