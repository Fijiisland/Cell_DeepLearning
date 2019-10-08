#ifndef GUIDEDIALOG_H
#define GUIDEDIALOG_H

#include "Kits/_utility.h"

#ifdef Q_OS_WIN32
#include "Kits/DropShadowDialog.h"
#endif

#include "Kits/customFrame.h"

namespace Ui {
class GuideDialog;
}

class GuideDialog : public DropShadowDialog
{
    Q_OBJECT

public:
    explicit GuideDialog(QWidget *parent = nullptr);
    ~GuideDialog();

private:
    Ui::GuideDialog   *ui;
    customFrame       *frame_titleBar;
    QPropertyAnimation *animi_main;
    QPropertyAnimation *animi_title;

private:
    QPoint  m_startPoint;
    QPoint  m_windowPoint;
    bool    m_move;

private:
    void Init();

private:
    COLOR_SCHEME m_mode;

protected:
#ifdef Q_OS_WIN32
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

    virtual void    mousePressEvent(QMouseEvent *event);

    virtual void    mouseMoveEvent(QMouseEvent *event);

    virtual void    mouseReleaseEvent(QMouseEvent *event);
private slots:
    void setColorScheme(COLOR_SCHEME);
    void on_Btn_close_clicked();
};

#endif // GUIDEDIALOG_H
