#include "Processing.h"
#include "../../CellCore/Kits/CellGlobalMacros.h"

ProcessingWidget::ProcessingWidget(QWidget *parent) noexcept:
    QWidget(parent),
    mDiam(10)
{
    init();
    setEventConnections();
}

void ProcessingWidget::init()
{
    setStyleSheet(CHAR2STR("background:transparent"));
    setFixedSize(500, 20);

    animi.setTargetObject(this);

    animi.setPropertyName("offset");
    animi.setEasingCurve(QEasingCurve::OutInCubic);
    animi.setDuration(150);

    animi.setStartValue(-mDiam);
    animi.setEndValue(width() + mDiam);
    animi.setDuration(2500);
    animi.setLoopCount(-1);
    animi.start(QAbstractAnimation::KeepWhenStopped);
}

void ProcessingWidget::paintEvent(QPaintEvent *event)
{
    (void)event;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    painter.drawEllipse(mOffset, 0, mDiam, mDiam);
    //painter.drawEllipse(mOffset - mDiam, 0, mDiam, mDiam);
    //painter.drawEllipse(mOffset - 2*mDiam, 0, mDiam, mDiam);
}

void ProcessingWidget::setEventConnections()
{

}
