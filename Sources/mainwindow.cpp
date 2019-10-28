// Copyright 2019 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QFontDatabase>
#include <QDebug>
#include <QTime>

#include "Headers/guidedialog.h"
#include "Headers/homepagewidget.h"
#include "Headers/settingspagewidget.h"
#include "Headers/workshop.h"
#include "Headers/mainwindow.h"
#include "Headers/Kits/qstylesheetloader.hpp"
#include "ui_mainwindow.h"
#define DEBUG

using namespace CELL_UTIL;

mainWindow::mainWindow(QWidget *parent)
    : DropShadowWidget(parent)
    , ui(new Ui::mainWindow)
    , mainWindowTabBtns(new QList<QPushButton*>)
    , workshop(nullptr)
    , homePage(new HomePageWidget)
    , settingsPage(new SettingsPageWidget)
    , currentPage(PAGE_TYPE::_HOME)
    , m_mode(COLOR_SCHEME::_BRIGHT)
{
    ui->setupUi(this);
    InitMainWindow();
}
mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::setColorScheme(COLOR_SCHEME mode)
{
    using TOOLS::styleSheetLoader;
    if(mode == COLOR_SCHEME::_BRIGHT){
        if(mode == m_mode) return;
        m_mode = COLOR_SCHEME::_BRIGHT;
        TOOLS::setPropertyAnimation({propertyAnimi}, "color", color(), LITERAL::MAINWINDOW_BRIGHT, 500,
                             QEasingCurve::InOutCubic, {this}, nullptr);
        TOOLS::setPropertyAnimation({propertyAnimi}, "color", frame_titleBar->color(), QColor(255,255,255), 500,
                             QEasingCurve::InOutCubic, {frame_titleBar}, nullptr);

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowMaxBtn_bright.css"));
        ui->Btn_max->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowMinimizeBtn_bright.css"));
        ui->Btn_mini->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowCloseBtn_bright.css"));
        ui->Btn_close->setStyleSheet(styleSheetLoader->styleSheet());

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_HomePage_Bright.css"));
        ui->Btn_HomePage->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Settings_Bright.css"));
        ui->Btn_Settings->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Guide_Bright.css"));
        ui->Btn_Guide->setStyleSheet(styleSheetLoader->styleSheet());

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_OpenPJ_Bright.css"));
        ui->Btn_OpenProject->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_NewPJ_Bright.css"));
        ui->Btn_NewProject->setStyleSheet(styleSheetLoader->styleSheet());
        QApplication::restoreOverrideCursor();
    }
    else{
        if(mode == m_mode) return;
        m_mode = COLOR_SCHEME::_DARK;
        TOOLS::setPropertyAnimation({propertyAnimi}, "color", color(), LITERAL::MAINWINDOW_DARK, 500,
                             QEasingCurve::InOutCubic, {this}, nullptr);
        TOOLS::setPropertyAnimation({propertyAnimi}, "color", frame_titleBar->color(), QColor(44,44,45), 500,
                             QEasingCurve::InOutCubic, {frame_titleBar}, nullptr);

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowMaxBtn_dark.css"));
        ui->Btn_max->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowMinimizeBtn_dark.css"));
        ui->Btn_mini->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowCloseBtn_dark.css"));
        ui->Btn_close->setStyleSheet(styleSheetLoader->styleSheet());

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_HomePage_Dark.css"));
        ui->Btn_HomePage->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Settings_Dark.css"));
        ui->Btn_Settings->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Guide_Dark.css"));
        ui->Btn_Guide->setStyleSheet(styleSheetLoader->styleSheet());

        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_OpenPJ_Dark.css"));
        ui->Btn_OpenProject->setStyleSheet(styleSheetLoader->styleSheet());
        styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_NewPJ_Dark.css"));
        ui->Btn_NewProject->setStyleSheet(styleSheetLoader->styleSheet());
        QApplication::restoreOverrideCursor();
    }
}

void mainWindow::InitMainWindow()
{
    // Functional.
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);  // Remove Windows' Default Window Frame.
#ifdef Q_OS_WIN32
    DropShadowWidget::LoadWinStyle(this);
#endif

    connect(settingsPage, SIGNAL(enableColorScheme(COLOR_SCHEME)),
            this, SLOT(setColorScheme(COLOR_SCHEME)), Qt::QueuedConnection);

    connect(settingsPage, SIGNAL(enableColorScheme(COLOR_SCHEME)),
            homePage, SLOT(setColorScheme(COLOR_SCHEME)), Qt::QueuedConnection);

    frame_titleBar = new customFrame(WINDOW_TYPE::_MAIN, this);
    frame_titleBar->setGeometry(0, 0, 1311, 61);
    frame_titleBar->setStyleSheet(QStringLiteral("QFrame{background-color:rgb(255,255,255);}"));

    ui->label_mainWindowIcon->setParent(frame_titleBar);
    ui->label_mainWindowTitle->setParent(frame_titleBar);
    ui->Btn_max->setParent(frame_titleBar);
    ui->Btn_mini->setParent(frame_titleBar);
    ui->Btn_close->setParent(frame_titleBar);


    mainWindowTabBtns->append(ui->Btn_HomePage);
    mainWindowTabBtns->append(ui->Btn_Settings);

    ui->stackedWidget->addWidget(homePage);
    ui->stackedWidget->addWidget(settingsPage);
    ui->stackedWidget->setCurrentWidget(homePage);

    // Load Custom Fonts.
    int fontID_Info = QFontDatabase::addApplicationFont(LITERAL::FONT_DIR + QStringLiteral("InfoDisplayWeb W01 Medium.ttf"));
    QString Info = QFontDatabase::applicationFontFamilies(fontID_Info).at(0);
#ifdef DEBUG
    qDebug() << QStringLiteral("Main Window Title Font: ") << Info;
#endif

    // Load Styles.
    using TOOLS::styleSheetLoader;
    // Window Title
    QFont font_Info(Info, 14);
    font_Info.setLetterSpacing(QFont::PercentageSpacing, 100);
    ui->label_mainWindowTitle->setFont(font_Info);
    ui->label_mainWindowTitle->setStyleSheet("color:"+LITERAL::COLOR_SPACE_GRAY+";");
    // Window Welcome Hint.
    ui->label_welcome->setFont(QFont(QStringLiteral("微软雅黑 Light"), 18));
    // Window Side Tabs.
    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_HomePage_Bright.css"));
    ui->Btn_HomePage->setStyleSheet(styleSheetLoader->styleSheet());

    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Settings_Bright.css"));
    ui->Btn_Settings->setStyleSheet(styleSheetLoader->styleSheet());

    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_Guide_Bright.css"));
    ui->Btn_Guide->setStyleSheet(styleSheetLoader->styleSheet());
    // Window Project Btns.
    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_NewPJ_Bright.css"));
    ui->Btn_NewProject->setStyleSheet(styleSheetLoader->styleSheet());

    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowLeftTab_OpenPJ_Bright.css"));
    ui->Btn_OpenProject->setStyleSheet(styleSheetLoader->styleSheet());
    // Window Opeartion Btns
    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowCloseBtn_bright.css"));
    ui->Btn_close->setStyleSheet(styleSheetLoader->styleSheet());

    styleSheetLoader->setStyleSheetName(QStringLiteral("MainWindowMinimizeBtn_bright.css"));
    ui->Btn_mini->setStyleSheet(styleSheetLoader->styleSheet());

    // Functional;
    guideDialog = new GuideDialog(this);
    connect(settingsPage, SIGNAL(enableColorScheme(COLOR_SCHEME)),
            guideDialog, SLOT(setColorScheme(COLOR_SCHEME)), Qt::QueuedConnection);
    guideDialog->show();

    QTime currentTime = QTime::currentTime();
    if(currentTime.hour() >= 18 && m_mode == COLOR_SCHEME::_BRIGHT){
        on_Btn_Settings_clicked();
        settingsPage->mainWindowSetColorSchemeModeCall(COLOR_SCHEME::_DARK);
    }else if(currentTime.hour() < 18 && m_mode == COLOR_SCHEME::_DARK){
        on_Btn_Settings_clicked();
        settingsPage->mainWindowSetColorSchemeModeCall(COLOR_SCHEME::_BRIGHT);
    }
}

void mainWindow::on_Btn_mini_clicked()
{
    // 在macOS下有bug
    if(this->windowState() != Qt::WindowMinimized)
            this->setWindowState(Qt::WindowMinimized);
}

void mainWindow::on_Btn_close_clicked()
{
    this->close();
    if(workshop){
        workshop->close();
    }
}

void mainWindow::on_Btn_HomePage_clicked()
{
    if(currentPage == PAGE_TYPE::_HOME)
        return;
    currentPage = PAGE_TYPE::_HOME;
    setAllTabsUnchecked();
    ui->Btn_HomePage->setChecked(true);
    startPageSwitchAnimation(PAGE_TYPE::_HOME);
}

void mainWindow::on_Btn_Settings_clicked()
{
    if(currentPage == PAGE_TYPE::_SETTINGS)
        return;
    currentPage = PAGE_TYPE::_SETTINGS;
    setAllTabsUnchecked();
    ui->Btn_Settings->setChecked(true);
    startPageSwitchAnimation(PAGE_TYPE::_SETTINGS);
}

void mainWindow::on_Btn_Guide_clicked()
{
    guideDialog->show();
}

void mainWindow::startFadeInOrOutAnimation(QWidget *target, int duration, FADE_TYPE type)
{
    int startValue = 0, endValue = 1;
    if(type == FADE_TYPE::_OUT)
        qSwap(startValue, endValue);
    opacityEffect = new QGraphicsOpacityEffect(target);
    opacityEffect->setOpacity(startValue);
    target->setGraphicsEffect(opacityEffect);
    TOOLS::setPropertyAnimation({propertyAnimi}, "opacity", startValue, endValue, duration,
                         QEasingCurve::Linear, {nullptr}, opacityEffect);
}

void mainWindow::setAllTabsUnchecked()
{
    for(auto & e : *mainWindowTabBtns)
        e->setChecked(false);
}

void mainWindow::startPageSwitchAnimation(PAGE_TYPE nextPage)
{
    int duration = 200;
    if(nextPage == PAGE_TYPE::_SETTINGS){
        settingsPage->setWindowOpacity(0);
        ui->stackedWidget->setCurrentWidget(settingsPage);
        startFadeInOrOutAnimation(settingsPage, duration, FADE_TYPE::_IN);
    }else{
        homePage->setWindowOpacity(0);
        ui->stackedWidget->setCurrentWidget(homePage);
        startFadeInOrOutAnimation(homePage, duration, FADE_TYPE::_IN);
    }
}

#ifdef Q_OS_WIN32
bool mainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    return DropShadowWidget::nativeEvent(eventType, message, result);
}
#endif
// Mouse drag process.
void mainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->y() <= 60){
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

void mainWindow::on_Btn_NewProject_clicked()
{
    workshop = new Workshop(m_mode);
    connect(settingsPage, SIGNAL(enableColorScheme(COLOR_SCHEME)),
            workshop, SLOT(setColorScheme(COLOR_SCHEME)), Qt::QueuedConnection);
    workshop->show();
    this->on_Btn_mini_clicked();
}
