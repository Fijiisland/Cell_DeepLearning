#include "Headers/mainwindow.h"
#include "Headers/WindWMAPI.h"
#include "ui_mainwindow.h"
#pragma comment(lib, "user32.lib")
#define DEBUG

mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWindow)
    , isMaxSize(false)
{
    ui->setupUi(this);
    InitMainWindow();
    Sleep(3000);
}
mainWindow::~mainWindow()
{
    delete ui;
}

// Achieve the window drop shadow effect.
bool mainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        *result = 0;
        return true;
    }
    default:
        return QWidget::nativeEvent(eventType, message, result);
    }
}

// Mouse drag process.
void mainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->y() <= 40){
        m_move = true;
        m_startPoint = event->globalPos();
        m_windowPoint = this->frameGeometry().topLeft();
    }
}

void mainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_move && event->buttons() & Qt::LeftButton){
        QPoint relativePos = event->globalPos() - m_startPoint;
        this->move(m_windowPoint + relativePos );
    }
}

void mainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_move = false;
    }
}
//

void mainWindow::InitMainWindow()
{
    // Functional.
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);  // Remove Windows' Default Window Frame.
    styleSheetLoader = new QFile;
    ui->Btn_max->setEnabled(false);

    // Achieve the window drop shadow effect.
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    const MARGINS shadow = { 1, 1, 1, 1 };
    WinDwmapi::instance()->DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

    // Load Custom Fonts.
    int fontID_Info = QFontDatabase::addApplicationFont(FONT_DIR + QStringLiteral("InfoDisplayWeb W01 Medium.ttf"));
    QString Info = QFontDatabase::applicationFontFamilies(fontID_Info).at(0);
#ifdef DEBUG
    qDebug() << QStringLiteral("Main Window Title Font: ") << Info;
#endif

    // Load Styles.
    QFont font_Lulo(Info, 14);
    font_Lulo.setLetterSpacing(QFont::PercentageSpacing, 100);
    ui->label_mainWindowTitle->setFont(font_Lulo);
    ui->label_mainWindowTitle->setStyleSheet("color:"+COLOR_SPACE_GRAY+";");

    setMyStyleSheet(QStringLiteral("MainWindowMinimizeBtn.qss"));
    ui->Btn_mini->setStyleSheet(myStyleSheet);

    setMyStyleSheet(QStringLiteral("MainWindowMaximizeBtn.qss"));
    ui->Btn_max->setStyleSheet(myStyleSheet);

    setMyStyleSheet(QStringLiteral("MainWindowCloseBtn.qss"));
    ui->Btn_close->setStyleSheet(myStyleSheet);
}

void mainWindow::setMyStyleSheet(QString name)
{
    styleSheetLoader->setFileName(STYLESHEET_DIR + name);
    styleSheetLoader->open(QFile::ReadOnly);
    myStyleSheet = tr(styleSheetLoader->readAll());
    styleSheetLoader->close();
}


void mainWindow::on_Btn_mini_clicked()
{
    if(this->windowState() != Qt::WindowMinimized)
            this->setWindowState(Qt::WindowMinimized);    // 在macOS下有bug
}

void mainWindow::on_Btn_max_clicked()
{
    static QSize size;
    static QPoint point;
    if(!isMaxSize){
        size.setWidth(width());
        size.setHeight(height());
        point = pos();

        QDesktopWidget *desktopWidget = QApplication::desktop();
        QRect rect = desktopWidget->availableGeometry();
        resize(rect.width(), rect.height());
        move(0, 0);
    }
}

void mainWindow::on_Btn_close_clicked()
{
    close();
}
