// Copyright 2018-2020 CellTek. < autologic@foxmail.com >
//
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the free software foundation and appearing in
// the file LICENSE included in the packaging of this file.
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

#include "../CustomBaseWidgets/customFrame.h"
#include "../CustomBaseWidgets/customSwitchFrame.h"
#include "../../CellCore/CellNamespace.h"
#include "../../CellCore/Kits/CellUtility.h"
#include "../../CellCore/CellVariant.h"
#include "../../CellCore/CellSqlManager.h"
#include "WSLoadingDialog.h"
#include "WorkShop.h"

Workshop::Workshop(Cell::ColorScheme mainWindow_mode, QWidget *parent) :
    QWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    loadingDialog(new WSLoadingDialog),
    menuBar(new QMenuBar(this)),
    leftBlock(new customFrame(customFrame::Type::Regular, this)),
    leftStackedWidget(new QStackedWidget(this)),
    btnDirectory(new QPushButton(this)),
    btnWarning(new QPushButton(this)),
    btnToolChain(new QPushButton(this)),
    treeView(new QTreeView(this)),
    fileModel(new QFileSystemModel(this)),
    mainEditor(new QsciScintilla(this)),
    rightBlock(new customFrame(customFrame::Type::Regular, this)),
    statusBar(new customFrame(customFrame::Type::Regular, this)),
    textChangetoken(new customSwitchFrame(Cell::CellThemeColor::NavyBlue ,this)),
    labelCntRow(new QLabel(this)),
    labelCntChar(new QLabel(this)),
    labelFormat(new QLabel(this)),
    ctrlS(new QShortcut(this))
{
    init();
    setEventConnections();
    if(CellWidgetGlobalInterface::mMode != mainWindow_mode)
        setColorScheme(mainWindow_mode);
}

void Workshop::init()
{
    // Functional.
    this->resize(1400, 820);
    setWindowTitle(CHAR2STR("WorkShop"));
    setAttribute(Qt::WA_DeleteOnClose);

    // Set MenuBar.
    CellUiGlobal::loader.setFileName(CHAR2STR("WorkshopMenuBar.css"));
    menuBar->setStyleSheet(CellUiGlobal::loader.content());
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
    initMainEditor();

    // Set TreeView.
    initTreeView();

    // Set LeftBlock.
    leftBlock->setBrightDarkColor(Cell::CGL247,Cell::CGL30);
    leftBlock->setMinimumWidth(300);
    QVBoxLayout *leftBlockLayout = new QVBoxLayout(leftBlock);
    customFrame *topBtnsFrame = new customFrame(customFrame::Type::Regular,leftBlock);
    topBtnsFrame->setFixedHeight(27);
    topBtnsFrame->setBrightDarkColor(Cell::CGL247,Cell::CGL30);

    QHBoxLayout *HLayout = new QHBoxLayout(topBtnsFrame);
    HLayout->setContentsMargins(8, 0, 0, 0);
    HLayout->setSpacing(13);
    HLayout->addStretch();
    HLayout->addWidget(btnDirectory);
    HLayout->addStretch();
    HLayout->addWidget(btnWarning);
    HLayout->addStretch();
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

    CellUiGlobal::loader.setFileName(CHAR2STR("WorkShopBtnDirectory.css"));
    btnDirectory->setStyleSheet(CellUiGlobal::loader.content());
    btnDirectory->setChecked(true);

    CellUiGlobal::loader.setFileName(CHAR2STR("WorkShopBtnWarrning.css"));
    btnWarning->setStyleSheet(CellUiGlobal::loader.content());

    CellUiGlobal::loader.setFileName(CHAR2STR("WorkShopBtnToolChain.css"));
    btnToolChain->setStyleSheet(CellUiGlobal::loader.content());

    leftStackedWidget->addWidget(treeView);
    leftStackedWidget->setCurrentIndex(0);

    leftBlockLayout->setMargin(0);
    leftBlockLayout->setSpacing(0);
    leftBlockLayout->addWidget(topBtnsFrame);
    leftBlockLayout->addWidget(CellUiGlobal::getLine(Cell::LineType::HLine));
    leftBlockLayout->addWidget(leftStackedWidget);
    leftBlock->setLayout(leftBlockLayout);

    // Set RightBlock
    rightBlock->setBrightDarkColor(Cell::CGL247,Cell::CGL30);
    rightBlock->setMinimumWidth(300);
    QVBoxLayout *rightBlockLayout = new QVBoxLayout(rightBlock);
    rightBlockLayout->setMargin(0);
    rightBlockLayout->addStretch(1);
    rightBlockLayout->addWidget(CellUiGlobal::getLine(Cell::LineType::HLine));
    rightBlockLayout->addStretch(25);
    rightBlock->setLayout(rightBlockLayout);

    // Set StatusBar.
    CellUiGlobal::setCustomTextLabel(labelCntRow,  CHAR2STR("Microsoft YaHei UI Light"), 12, CHAR2STR("Row: 0"));
    CellUiGlobal::setCustomTextLabel(labelCntChar, CHAR2STR("Microsoft YaHei UI Light"), 12, CHAR2STR("Characters: 0"));
    CellUiGlobal::setCustomTextLabel(labelFormat,  CHAR2STR("Microsoft YaHei UI Light"), 12, CHAR2STR("UTF-8"));

    textChangetoken->setBrightDarkColor(Cell::CellThemeColor::pureGreen, Cell::CGL45);
    textChangetoken->setFixedSize(23, 15);

    statusBar->setFixedHeight(25);
    statusBar->setBrightDarkColor(Cell::CGL218, Cell::CGL45);

    CellWidgetGlobalInterface::_modules << textChangetoken << statusBar << leftBlock << rightBlock
             << topBtnsFrame;

    QHBoxLayout *HLayoutStatusBar = new QHBoxLayout(statusBar);
    HLayoutStatusBar->setContentsMargins(22, 0, 17, 0);
    HLayoutStatusBar->setSpacing(15);
    HLayoutStatusBar->addWidget(labelCntRow);
    HLayoutStatusBar->addWidget(labelCntChar);
    HLayoutStatusBar->addStretch();
    HLayoutStatusBar->addWidget(labelFormat);
    HLayoutStatusBar->addWidget(textChangetoken);
    statusBar->setLayout(HLayoutStatusBar);

    // Functional.
    loadingDialog->show();
    loadingDialog->progress();

    ctrlS->setKey(tr("ctrl+s"));
    ctrlS->setAutoRepeat(false);

    mainLayout->setSpacing(0);
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(menuBar);
    mainLayout->addWidget(CellUiGlobal::getLine(Cell::LineType::HLine));
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(CellUiGlobal::getLine(Cell::LineType::HLine));
    mainLayout->addWidget(statusBar);
}

void Workshop::setEventConnections()
{
    connect(ctrlS, &QShortcut::activated     , this, &Workshop::saveFile);
    connect(mainEditor, SIGNAL(textChanged()), this, SLOT(updateStatusBar()));
    connect(mainEditor, SIGNAL(textChanged()), this, SLOT(checkCodeModifiedState()));
}

void Workshop::initMainEditor()
{
    QsciLexer *lexCPP = new QsciLexerCPP(mainEditor);
    lexCPP->setFont(QFont(CHAR2STR("Courier New"), 10));
    lexCPP->setPaper(CellVariant(Cell::CGL245).toColor());
    mainEditor->setLexer(lexCPP);
    mainEditor->setUtf8(true);
    mainEditor->setMinimumWidth(500);
    mainEditor->setCaretLineVisible(true);
    mainEditor->setMarginWidth(0, 55);
    mainEditor->setMarginsBackgroundColor(CellVariant(Cell::CGL245).toColor());
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

    mainEditor->setCaretLineBackgroundColor(CellVariant(Cell::CellThemeColor::yellowGreen).toColor());
    mainEditor->setAutoCompletionCaseSensitivity(false);
    mainEditor->setAutoCompletionThreshold(3);
    mainEditor->setAutoCompletionSource(QsciScintilla::AcsAll);
    mainEditor->setAutoCompletionReplaceWord(false);

    CellUiGlobal::loader.setFileName(CHAR2STR("WorkshopEditorVerticalScrollBar.css"));
    QScrollBar *verticalBar = mainEditor->verticalScrollBar();
    verticalBar->setStyleSheet(CellUiGlobal::loader.content());

    CellUiGlobal::loader.setFileName(CHAR2STR("WorkshopEditorHorizontalScrollBar.css"));
    QScrollBar *horizontalBar = mainEditor->horizontalScrollBar();
    horizontalBar->setStyleSheet(CellUiGlobal::loader.content());
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

void Workshop::updateStatusBar()
{
    labelCntRow->setText("Row: " + QString::number(mainEditor->lines()));

    QString tmp = mainEditor->text();
    labelCntChar->setText("Char: " + QString::number(tmp.length()));
}

void Workshop::saveFile()
{
    textChangetoken->transCurrState(customSwitchFrame::Normal);

    currEntity.setCode(mainEditor->text());

    CellSqlManager manager;
    manager.setDataBase("CellDB.db");
    if(!manager.tupleExists(CHAR2STR("RecentPJ"), CHAR2STR("Name"), currEntity.name())){
        emit projectUpdate(currEntity);
        manager.insertProjectEntity(currEntity);
    }

    QJsonObject Object;
    write(Object);
    QJsonDocument saveDoc(Object);

    QFile saveFile(currEntity.path() + CHAR2STR("\\") + currEntity.name() + CHAR2STR(".workshop"));
    if(!saveFile.open(QIODevice::WriteOnly)){
        qWarning("Cannot Open File");
    }
    saveFile.write(saveDoc.toBinaryData());
    saveFile.close();
}

void Workshop::loadFile(const QString &path)
{
#ifdef CELL_DEBUG
       CELL_DEBUG("WorkShop") << "Project_Path: " << path << endl;
#endif
    QFile loadProject(path);
    if (!loadProject.open(QIODevice::ReadOnly))
        qWarning("Couldn't open save file.");
    QByteArray saveData = loadProject.readAll();
    loadProject.close();
    QJsonDocument loadDoc(QJsonDocument::fromBinaryData(saveData));
    read(loadDoc.object());

    currEntity.setPath(path.left(path.lastIndexOf('/')));

    mainEditor->setText(currEntity.code());

    fileModel->setRootPath(currEntity.path());
    treeView->setRootIndex(fileModel->index(currEntity.path()));
}

void Workshop::newProject(CellProjectEntity &entity)
{
    currEntity = entity;
    fileModel->setRootPath(currEntity.path());
    treeView->setRootIndex(fileModel->index(currEntity.path()));
    saveFile();
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

void Workshop::checkCodeModifiedState()
{
    currEntity.code() == mainEditor->text() ?
        codeModified = false:
        codeModified = true;
    codeModified ?
        textChangetoken->transCurrState(customSwitchFrame::Special):
        textChangetoken->transCurrState(customSwitchFrame::Normal);
}

void Workshop::setColor(const QColor& color)
{
    CellWidgetGlobalInterface::setColor(color);
    setStyleSheet(QString("background-color:rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void Workshop::changeToColor(const QColor &startColor, const QColor &targetColor, int duration)
{
    CellWidgetGlobalInterface::switchMode == Cell::SwitchMode::Instant ?
    setColor(targetColor):
    CellUiGlobal::setPropertyAnimation(animi, this, "color",
                                       startColor, targetColor, duration,
                                       CellWidgetGlobalInterface::easingCurve);
}

void Workshop::closeEvent(QCloseEvent* e)
{
    (void)e;
    emit destoryed();
    e->accept();
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

void Workshop::setColorScheme(Cell::ColorScheme mode){
    CellWidgetGlobalInterface::setColorScheme(mode);
}
