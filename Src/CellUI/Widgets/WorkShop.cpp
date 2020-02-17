// Copyright 2018-2019 CellTek.
//
// Distributed under the GPL License, Version 3.0.
//
// See accompanying file LICENSE.txt at the root
//
// Of source file directory.
#include <QLabel>
#include <QDebug>
#include <QCursor>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QShortcut>
#include <QMenuBar>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QFileSystemModel>
#include <QStandardItem>
#include <QMenu>
#include <QFile>
#include <QStackedWidget>
#include <QStringList>
#include <QScrollBar>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexercpp.h>
#include <QButtonGroup>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>

#include "../CustomBaseWidgets/customFrame.h"
#include "../CustomBaseWidgets/customGradientChangeFrame.h"
#include "../../CellCore/Kits/CellUtility.h"
#include "../../CellCore/Kits/StyleSheetLoader.hpp"
#include "../../CellCore/CellSqlManager.h"
#include "WSLoadingDialog.h"
#include "WorkShop.h"

Workshop::Workshop(CellUiGlobal::COLOR_SCHEME mainWindow_mode, QWidget *parent) :
    QWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    loadingDialog(new WSLoadingDialog),
    menuBar(new QMenuBar(this)),
    leftBlock(new customFrame(customFrame::_REGULAR, this)),
    rightBlock(new customFrame(customFrame::_REGULAR, this)),
    statusBar(new customGradientChangeFrame(CellUiConst::CELLTHEMECOLOR ,this)),
    mainEditor(new QsciScintilla(this)),
    leftStackedWidget(new QStackedWidget(leftBlock)),
    treeView(new QTreeView(leftBlock)),
    fileModel(new QFileSystemModel(this)),
    btnDirectory(new QPushButton(this)),
    btnWarning(new QPushButton(this)),
    btnToolChain(new QPushButton(this)),
    labelCntRow(new QLabel(statusBar)),
    labelCntChar(new QLabel(statusBar)),
    labelFormat(new QLabel(statusBar)),
    ctrlS(new QShortcut(this))
{
    init();
    setEventConnections();
    if(m_mode != mainWindow_mode)
        setColorScheme(mainWindow_mode);
}

void Workshop::init()
{
    // Functional.
    this->resize(1400, 800);
    setWindowTitle(CHAR2STR("WorkShop"));
    setWindowFlag(Qt::WindowType::Window);

    // Set MenuBar.
    using CellEntityTools::styleSheetLoader;
    styleSheetLoader->setStyleSheetName(CHAR2STR("WorkshopMenuBar.css"));
    menuBar->setStyleSheet(styleSheetLoader->styleSheet());
    menuBar->setFont(CellUiGlobal::getFont(CHAR2STR("Microsoft YaHei UI"), 15));
    menuBar->setFixedHeight(27);

    QMenu *fileMenu = new QMenu(menuBar);
    fileMenu->setTitle("文件(F)");
    fileMenu->addAction("OpenFile");

    QMenu *editMenu = new QMenu(menuBar);
    editMenu->setTitle("编辑(E)");

    QMenu *buildMenu = new QMenu(menuBar);
    buildMenu->setTitle("构建(B)");

    QMenu *helpMenu = new QMenu(menuBar);
    helpMenu->setTitle("构建(D)");

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(buildMenu);
    menuBar->addMenu(helpMenu);

    // Set Splitter.
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->setOpaqueResize(true);
    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setStyleSheet(CHAR2STR("QSplitter::handle{background-color:grey;}"));
    splitter->addWidget(leftBlock);
    splitter->addWidget(mainEditor);
    splitter->addWidget(rightBlock);
    splitter->setCollapsible(0,true);
    splitter->setCollapsible(1,false);
    splitter->setCollapsible(2,true);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    splitter->setStretchFactor(2, 1);


    // Set MainEditor.
    QsciLexer *lexCPP = new QsciLexerCPP();
    lexCPP->setFont(QFont(CHAR2STR("Courier New"), 10));
    mainEditor->setLexer(lexCPP);
    mainEditor->setUtf8(true);
    mainEditor->setMinimumWidth(500);
    mainEditor->setCaretLineVisible(true);
    mainEditor->setMarginWidth(0, 55);
    mainEditor->setMarginsBackgroundColor(QColor(240,240,240));
    mainEditor->setIndentationsUseTabs(true);
    mainEditor->setIndentationGuides(true);
    mainEditor->setAutoIndent(true);
    mainEditor->setWrapIndentMode(QsciScintilla::WrapIndentFixed);
    mainEditor->setWrapMode(QsciScintilla::WrapCharacter);
    mainEditor->setWrapVisualFlags(QsciScintilla::WrapFlagInMargin, QsciScintilla::WrapFlagInMargin, 0);
    mainEditor->setTabWidth(4);
    mainEditor->setCaretLineVisible(true);
    mainEditor->setFrameShape(QFrame::NoFrame);
    mainEditor->setFont(CellUiGlobal::getFont(CHAR2STR("Courier New"), 14));
    mainEditor->setPaper(QColor(249,250,250));

    mainEditor->setCaretLineBackgroundColor(QColor(240,240,240));
    mainEditor->setAutoCompletionCaseSensitivity(false);
    mainEditor->setAutoCompletionThreshold(3);
    mainEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
    mainEditor->setAutoCompletionReplaceWord(false);

    CellEntityTools::styleSheetLoader->setStyleSheetName(CHAR2STR("WorkshopEditorVerticalScrollBar.css"));
    QScrollBar *verticalBar = mainEditor->verticalScrollBar();
    verticalBar->setStyleSheet(CellEntityTools::styleSheetLoader->styleSheet());

    CellEntityTools::styleSheetLoader->setStyleSheetName(CHAR2STR("WorkshopEditorHorizontalScrollBar.css"));
    QScrollBar *horizontalBar = mainEditor->horizontalScrollBar();
    horizontalBar->setStyleSheet(CellEntityTools::styleSheetLoader->styleSheet());

    // Set TreeView.
    initTreeView();

    // Set LeftBlock.
    leftBlock->setMinimumWidth(300);
    QVBoxLayout *leftBlockLayout = new QVBoxLayout(leftBlock);
    customFrame *topBtnsFrame = new customFrame(customFrame::_REGULAR,leftBlock);
    topBtnsFrame->setFixedHeight(27);

    QHBoxLayout *HLayout = new QHBoxLayout(topBtnsFrame);
    HLayout->setContentsMargins(8, 0, 0, 0);
    HLayout->setSpacing(13);
    HLayout->addWidget(btnDirectory);
    HLayout->addWidget(btnWarning);
    HLayout->addWidget(btnToolChain);
    HLayout->addStretch();

    QButtonGroup *leftButtonGroup = new QButtonGroup(topBtnsFrame);
    leftButtonGroup->setExclusive(true);

    QList<QPushButton*> leftBtnsList;
    leftBtnsList.append(btnDirectory);
    leftBtnsList.append(btnWarning);
    leftBtnsList.append(btnToolChain);
    for(auto & e : leftBtnsList){
        leftButtonGroup->addButton(e);
        e->setCursor(Qt::PointingHandCursor);
        e->setFixedSize(19, 19);
        e->setCheckable(true);
    }

    styleSheetLoader->setStyleSheetName(CHAR2STR("WorkShopBtnDirectory.css"));
    btnDirectory->setStyleSheet(styleSheetLoader->styleSheet());
    btnDirectory->setChecked(true);

    styleSheetLoader->setStyleSheetName(CHAR2STR("WorkShopBtnWarrning.css"));
    btnWarning->setStyleSheet(styleSheetLoader->styleSheet());

    styleSheetLoader->setStyleSheetName(CHAR2STR("WorkShopBtnToolChain.css"));
    btnToolChain->setStyleSheet(styleSheetLoader->styleSheet());

    leftStackedWidget->addWidget(treeView);
    leftStackedWidget->setCurrentIndex(0);

    leftBlockLayout->setMargin(0);
    leftBlockLayout->setSpacing(0);
    leftBlockLayout->addWidget(topBtnsFrame);
    leftBlockLayout->addWidget(CellUiGlobal::getLine(CellUiGlobal::LINE_TYPE::HLine));
    leftBlockLayout->addWidget(leftStackedWidget);
    leftBlock->setLayout(leftBlockLayout);

    // Set RightBlock
    rightBlock->setMinimumWidth(300);
    QVBoxLayout *rightBlockLayout = new QVBoxLayout(rightBlock);
    rightBlockLayout->setMargin(0);
    rightBlockLayout->addStretch(1);
    rightBlockLayout->addWidget(CellUiGlobal::getLine(CellUiGlobal::LINE_TYPE::HLine));
    rightBlockLayout->addStretch(25);
    rightBlock->setLayout(rightBlockLayout);

    // Set StatusBar.
    statusBar->setFixedHeight(25);
    statusBar->setBrightDarkModeColor(CellUiConst::GRAYLEVEL218, CellUiConst::GRAYLEVEL45);

    // Set Labels Of StatusBar.
    labelCntRow->move(370, 0);
    labelCntRow->setFont(QFont(CHAR2STR("Microsoft YaHei UI Light")));
    labelCntRow->setText(CHAR2STR("Row: 0"));

    labelCntChar->move(450, 0);
    labelCntChar->setFont(QFont(CHAR2STR("Microsoft YaHei UI Light")));
    labelCntChar->setText(CHAR2STR("Char: 0"));

    labelFormat->move(1030 - labelFormat->width(), 0);
    labelFormat->setFont(QFont(CHAR2STR("Microsoft YaHei UI Light")));
    labelFormat->setText(CHAR2STR("Format: UTF-8"));

    // Two Blocks.
    CellUiGlobal::multiModulesOneStyleSheet({leftBlock, rightBlock},
                                       CHAR2STR("QFrame{background-color:rgb(235,235,235);}"));

    // Functional.
    loadingDialog->show();
    loadingDialog->progress();

    codePrev = "#!/usr/bin/python\n"
           "#conding=utf-8\n"
           "import sys\n";
    mainEditor->setText(codePrev);

    ctrlS->setKey(tr("ctrl+s"));
    ctrlS->setAutoRepeat(false);

    mainLayout->setSpacing(0);
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(statusBar);
}

void Workshop::setEventConnections()
{
    connect(mainEditor, SIGNAL(textChanged()), this, SLOT(updateStatusBar()));
    connect(ctrlS, SIGNAL(activated()), this, SLOT(saveFile()));
    connect(mainEditor, SIGNAL(textChanged()), this, SLOT(checkCodeModifiedState()));
}

void Workshop::initTreeView()
{
    QStringList headerList;
    headerList << CHAR2STR("Project Directory");
    QStandardItemModel *itemModal = new QStandardItemModel(treeView);
    itemModal->setHorizontalHeaderLabels(headerList);
    treeView->setModel(fileModel);
    treeView->header()->setModel(itemModal);
    treeView->setFrameShape(QFrame::NoFrame);
    treeView->setAnimated(true);
    treeView->setIndentation(20);
    treeView->setSortingEnabled(false);
    treeView->header()->setDefaultAlignment(Qt::AlignmentFlag::AlignCenter);
    treeView->header()->setFont(CellUiGlobal::getFont(CHAR2STR("Microsoft YaHei UI Light"), 15));
    treeView->setFont(QFont(CHAR2STR("Microsoft YaHei UI Light")));
}

void Workshop::getProjectEntity(CellProjectEntity &entity)
{
    currEntity = entity;
    fileModel->setRootPath(currEntity.path());
    treeView->setRootIndex(fileModel->index(currEntity.path()));
}

void Workshop::setColorScheme(CellUiGlobal::COLOR_SCHEME mode)
{
    CellWidgetGlobalInterface::setColorScheme(mode);
}

void Workshop::updateStatusBar()
{
    labelCntRow->setText("Row: " + QString::number(mainEditor->lines()));

    QString tmp = mainEditor->text();
    labelCntChar->setText("Char: " + QString::number(tmp.length()));
}

void Workshop::loadFile(const QString &path)
{
#ifdef CELL_DEBUG
    CELL_DEBUG("WorkShop") << "Project_Path: " << path << endl;
#endif
    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly))
        qWarning("Couldn't open save file.");
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromBinaryData(saveData));
    read(loadDoc.object());

    mainEditor->setText(currEntity.code());

    fileModel->setRootPath(currEntity.path());
    treeView->setRootIndex(fileModel->index(currEntity.path()));
}

void Workshop::write(QJsonObject &json)
{
    QJsonObject projectObject;
    currEntity.write(projectObject);
    json["CellProject"] = projectObject;
}

void Workshop::read(const QJsonObject &json)
{
     if (json.contains("CellProject") && json["CellProject"].isObject())
         currEntity.read(json["CellProject"].toObject());
     currEntity.print();
}

void Workshop::saveFile()
{
    if(!codeModified) return;
    codePrev = codeCurr;
    statusBar->transCurrState(customGradientChangeFrame::_NORMAL);

    currEntity.setCode(codeCurr);

    CellSqlManager manager;
    manager.setDbPath("CellDB.db");
    manager.insertProjectEntity(currEntity);

    QJsonObject Object;
    write(Object);
    QJsonDocument saveDoc(Object);

    QFile saveFile(currEntity.path() + CHAR2STR("\\") + currEntity.name() + CHAR2STR(".workshop"));
    if(!saveFile.open(QIODevice::WriteOnly)){
        qWarning("Cannot Open File");
    }
    saveFile.write(saveDoc.toBinaryData());
    saveFile.close();

    emit projectUpdate(currEntity);
}

void Workshop::checkCodeModifiedState()
{
    codePrev == (codeCurr = mainEditor->text()) ?
    codeModified = false : codeModified = true;
    codeModified ?
        statusBar->transCurrState(customGradientChangeFrame::_SPECIAL):
        statusBar->transCurrState(customGradientChangeFrame::_NORMAL);
#ifdef CELL_DEBUG
    CELL_DEBUG("WorkShop") << CHAR2STR("Code_Modified") << endl;
#endif
}

void Workshop::setColor(const QColor& color)
{
    CellWidgetGlobalInterface::setColor(color);
    setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void Workshop::setBaseQss(const QString &qss)
{
    (void)qss;
}

void Workshop::changeToColor(const QColor &startColor, const QColor &targetColor, int duration)
{
    CellUiGlobal::setPropertyAnimation({animi},
                                 "color",
                                 startColor,
                                 targetColor,
                                 duration,
                                 easingCurve,
                                 {this},nullptr);
}

void Workshop::closeEvent(QCloseEvent*)
{
    emit destoryed();
}

void Workshop::btnDirectoryClicked()
{
    leftStackedWidget->setCurrentIndex(0);
}

void Workshop::btnWarrningClicked()
{
    leftStackedWidget->setCurrentIndex(1);
}

void Workshop::btnToolChainClicked()
{
    leftStackedWidget->setCurrentIndex(2);
}
