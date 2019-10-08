#include "../../Headers/Kits/DropShadowWidget.h"
#include "../../Headers/Kits/WindWMAPI.h"

DropShadowWidget::DropShadowWidget(QWidget *parent):
    QWidget(parent)
{
    m_color = QColor(247, 247, 247);
}

const QColor DropShadowWidget::color() const
{
    return m_color;
}

void DropShadowWidget::setColor(const QColor color)
{
    m_color = color;
    QString qss=QString("QWidget{background-color: rgb(%1, %2, %3);}").arg(color.red()).arg(color.green()).arg(color.blue());
    setStyleSheet(qss);
}

const QColor DropShadowWidget::b_color() const
{
    return bright;
}

const QColor DropShadowWidget::d_color() const
{
    return dark;
}
#ifdef Q_OS_WIN32
bool DropShadowWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    switch (msg->message){
        case WM_NCCALCSIZE:{
            *result = 0;
            return true;
        }
        default:
            return QWidget::nativeEvent(eventType, message, result);
    }
}
#endif

void DropShadowWidget::LoadWinStyle(QWidget *obj)
{
#ifdef Q_OS_WIN32
    // Achieve the window drop shadow effect.
    HWND hwnd =  (HWND)obj->winId();
    DWORD style = static_cast<DWORD>(::GetWindowLong(hwnd, GWL_STYLE));
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    const MARGINS shadow = { 1, 1, 1, 1 };
    WinDwmapi::instance()->DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
#endif
}