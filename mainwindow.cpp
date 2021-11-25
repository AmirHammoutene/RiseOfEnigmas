#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QColorDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QPushButton>
#include <QToolButton>
#include <QMenuBar>
#include <QScrollArea>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Rise Of Enigmas");

    // Load window settings from .ini file (geometry,...)
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::defaultFormat(), QSettings::UserScope,
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)); // Most settings stored in a ini file in AppData
    QSettings settings;
    if(!restoreGeometry(settings.value("mainWindowGeometry").toByteArray()) )
    {
        showMaximized();
    }
    restoreState(settings.value("mainWindowState").toByteArray());

    constructData();
    finishCreate();

    // Load other options (style, Eulerian Graphs line color,...)
    THstyleName = settings.value("style", "").toString();
    if(THstyleName.isEmpty())
    {
        THstyleName = "dark";
    }
    if(THstyleName == "dark")
        applyDarkStyle();
    else
        applyDefaultStyle();

    EGcustomColorPosition = 1;
    EGlineColor = QColor(settings.value("EulerGraphLineColor", QColor( Qt::GlobalColor::darkBlue ) ).toString());
    QColorDialog::setCustomColor(0, EGlineColor);
    emit EGlineColorRequest(EGlineColor);

    EGclickbyClickMode = settings.value("EulerGraphClickByClickMode", false).toBool();
    EGeasyMode->setChecked(EGclickbyClickMode);
    changeEGClickMode(EGclickbyClickMode);

    // Load game scores from registry
    QSettings scores(QSettings::UserScope, "Amir Hammoutene", "Rise Of Enigmas");
    EGscore = scores.value("EulerGraphScore",0).toUInt();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::finishCreate()
{
    statusBar()->hide();

    stackedWidget = new QStackedWidget(centralWidget());

    /// MENUBAR

    QMenuBar *menubar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu(tr("File"), menubar);
    QAction *closeAction = new QAction(tr("Quit"),menubar);
    QObject::connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction(closeAction);

    QMenu *toolsMenu = new QMenu(tr("Tools"), menubar);
    QMenu *resetMenu = new QMenu(tr("Reset Levels"), menubar);
    QAction *resetEGAction = new QAction(tr("Eulerian Graphs..."),menubar);
    QObject::connect( resetEGAction, SIGNAL(triggered()), this, SLOT( resetEulerGraphScore() ) );
    resetMenu->addAction(resetEGAction);
    QMenu *optionMenu = new QMenu(tr("Options"), menubar);
    QMenu *styleMenu = new QMenu(tr("Select Theme"), menubar);
    THselectDefaultStyleAction = new QAction("default",menubar);
    QObject::connect(THselectDefaultStyleAction, SIGNAL(triggered()), this, SLOT(applyDefaultStyle()));
    THselectDefaultStyleAction->setCheckable(true);
    styleMenu->addAction(THselectDefaultStyleAction);
    THselectDarkStyleAction = new QAction("dark",menubar);
    QObject::connect(THselectDarkStyleAction, SIGNAL(triggered()), this, SLOT(applyDarkStyle()));
    THselectDarkStyleAction->setCheckable(true);
    styleMenu->addAction(THselectDarkStyleAction);
    QAction * EGLineColorAction = new QAction(tr("Change Eulerian Graphs lines' color..."),menubar);
    QObject::connect(EGLineColorAction, SIGNAL(triggered()), this, SLOT(chooseEGlineColor()));
    optionMenu->addMenu(styleMenu);
    optionMenu->addAction(EGLineColorAction);
    toolsMenu->addMenu(resetMenu);
    toolsMenu->addMenu(optionMenu);

    QMenu *helpMenu = new QMenu(tr("Help"), menubar);
    QAction * aboutAction = new QAction(tr("About..."),menubar);
    QObject::connect(aboutAction, SIGNAL(triggered()), this, SLOT( onAbout()));
    QAction * readmeAction = new QAction(tr("Open readme.txt..."),menubar);
    QObject::connect(readmeAction, SIGNAL(triggered()), this, SLOT( onReadme()));
    QMenu *linksMenu = new QMenu(tr("Links"), helpMenu);
    QAction * dowloadLink1Action = new QAction(tr("Download page on gamejolt.com..."),linksMenu);
    QObject::connect(dowloadLink1Action, SIGNAL(triggered()), this, SLOT( onDownloadLink1()));
    linksMenu->addAction(dowloadLink1Action);
    QAction * dowloadLink2Action = new QAction(tr("Download page on itch.io..."),linksMenu);
    QObject::connect(dowloadLink2Action, SIGNAL(triggered()), this, SLOT( onDownloadLink2()));
    linksMenu->addAction(dowloadLink2Action);
    QAction * sourceLinkAction = new QAction(tr("Sources page on github.com..."),linksMenu);
    QObject::connect(sourceLinkAction, SIGNAL(triggered()), this, SLOT( onSourcesLink()));
    linksMenu->addAction(sourceLinkAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(readmeAction);
    helpMenu->addMenu(linksMenu);

    menubar->addMenu(fileMenu);
    menubar->addMenu(toolsMenu);
    menubar->addMenu(helpMenu);

    setMenuBar(menubar);

    /// HOME PAGE

    QScrollArea *HPmainWidget = new QScrollArea(stackedWidget);
    QGridLayout *HPmainLayout = new QGridLayout(HPmainWidget);

    QPushButton *HPEulerGraphButton = new QPushButton(QIcon(":/img/EulerGraph.png"),"",HPmainWidget);
    HPEulerGraphButton->setIconSize(QSize(210,243));
    HPEulerGraphButton->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    QLabel *HPEulerGraphLabel = new QLabel(tr("Eulerian Graphs"),HPmainWidget);
    HPEulerGraphLabel->setFont(QFont("Helvetica", 14,75));
    HPEulerGraphLabel->setAlignment(Qt::AlignCenter);
    HPEulerGraphLabel->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);

    QVBoxLayout *HPEulerGraphLayout = new QVBoxLayout(HPmainWidget);
    HPEulerGraphLayout->addWidget(HPEulerGraphButton);
    HPEulerGraphLayout->addSpacing(30);
    HPEulerGraphLayout->addWidget(HPEulerGraphLabel);

    QPushButton *HPcomingSoonButton = new QPushButton(QIcon(":/img/comingSoon.png"),"",HPmainWidget);
    HPcomingSoonButton->setIconSize(QSize(210,243));
    HPcomingSoonButton->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    HPcomingSoonButton->setEnabled(false);
    QLabel *HPcomingSoonLabel = new QLabel(tr("coming soon..."),HPmainWidget);
    HPcomingSoonLabel->setFont(QFont("Helvetica", 14,75));
    HPcomingSoonLabel->setAlignment(Qt::AlignCenter);
    HPcomingSoonLabel->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);

    QVBoxLayout *HPcomingSoonLayout = new QVBoxLayout(HPmainWidget);
    HPcomingSoonLayout->addWidget(HPcomingSoonButton);
    HPcomingSoonLayout->addSpacing(30);
    HPcomingSoonLayout->addWidget(HPcomingSoonLabel);

    HPmainLayout->addItem(new QSpacerItem(40,40),0,0);
    HPmainLayout->addLayout(HPEulerGraphLayout,0,1);
    HPmainLayout->addItem(new QSpacerItem(40,40),0,2);
    HPmainLayout->addLayout(HPcomingSoonLayout,0,3);
    HPmainLayout->addItem(new QSpacerItem(40,40),0,4);
    HPmainLayout->setColumnStretch(5,1);
    HPmainLayout->setRowStretch(1,1);

    QObject::connect(HPEulerGraphButton, SIGNAL(released()), this, SLOT(goToEulerGraphPage()));

    stackedWidget->addWidget(HPmainWidget);

    /// EULERIAN GRAPHS PAGE

    QScrollArea *EGmainWidget = new QScrollArea(stackedWidget);
    EGstateLabel = new QLabel(EGmainWidget);
    EGstateLabel->setFont(QFont("Helvetica", 14,75));
    EGstateLabel->setWordWrap(true);
    EGstateLabel->setSizePolicy(QSizePolicy::Policy::MinimumExpanding,QSizePolicy::Policy::Minimum);
    EGstateLabel->setMinimumWidth(500);
    EGeasyMode = new QCheckBox(tr("Click by Click Mode"),EGmainWidget);
    EGeasyMode->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    EGeasyMode->setFont(QFont("Helvetica", 14,100));
    QToolButton  *EGchangeLineColorButton = new QToolButton(HPmainWidget);
    EGchangeLineColorButton->setFont(QFont("Helvetica", 11,45));
    EGchangeLineColorButton->setDefaultAction(EGLineColorAction);
    QToolButton  *EGresetButton = new QToolButton(HPmainWidget);
    EGresetButton->setFont(QFont("Helvetica", 11,45));
    EGresetButton->setDefaultAction(resetEGAction);
    EGresetButton->setText(tr("Reset Levels"));
    QPushButton *EGhomeButton = new QPushButton(tr("Back to Menu"),EGmainWidget);
    EGhomeButton->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    EGhomeButton->setFont(QFont("Helvetica", 11,45));

    EGtimeChallengeWidget = new EulerGraphsTimeChallengeWidget(EGmainWidget);

    eulerGraph = new EulerGraphInteract(EGmainWidget);

    QGridLayout *EGmainLayout = new QGridLayout(EGmainWidget);
    EGmainLayout->addItem(new QSpacerItem(10,10),0,0);
    EGmainLayout->addWidget(EGtimeChallengeWidget,0,1);
    EGmainLayout->addItem(new QSpacerItem(10,10),0,2);
    EGmainLayout->addWidget(eulerGraph,1,1);
    EGmainLayout->addItem(new QSpacerItem(10,10),2,1);

    QHBoxLayout *EGtextAndButtonLayout = new QHBoxLayout(EGmainWidget);
    EGtextAndButtonLayout->addWidget(EGstateLabel);
    EGtextAndButtonLayout->addStretch(0);
    EGtextAndButtonLayout->addWidget(EGeasyMode);
    EGtextAndButtonLayout->addStretch(0);
    EGtextAndButtonLayout->addWidget(EGchangeLineColorButton);
    EGtextAndButtonLayout->addWidget(EGresetButton);
    EGtextAndButtonLayout->addWidget(EGhomeButton);

    EGmainLayout->addLayout(EGtextAndButtonLayout,3,1);
    EGmainLayout->addItem(new QSpacerItem(40,40),4,1);

    EGmainWidget->setLayout(EGmainLayout);

    QObject::connect( &eulerGraph->m_scene, SIGNAL( stepedUp(uint,uint) ), this, SLOT( EGStepedUp(uint,uint) ) );
    QObject::connect( eulerGraph, SIGNAL( fullFinishedGraph() ), this, SLOT( setNextEulerGraph() ) );
    QObject::connect( EGhomeButton, SIGNAL( released() ), this, SLOT( goToHomehPage() ) );
    QObject::connect( EGeasyMode, SIGNAL( stateChanged(int) ), this, SLOT( changeEGClickMode(int) ) );
    QObject::connect( this, SIGNAL( EGlineColorRequest(QColor)) , &eulerGraph->m_scene, SLOT( changeLineColor(QColor) ) ) ;
    QObject::connect( EGtimeChallengeWidget, SIGNAL( startChallengeRequest()) , this, SLOT( EGstartChallenge() ) ) ;

    ///

    stackedWidget->addWidget(EGmainWidget);
    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentIndex(0);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save game scores in registry
    QSettings scores(QSettings::UserScope, "Amir Hammoutene", "Rise Of Enigmas");
    scores.setValue("EulerGraphScore", EGscore);

    // Save window settings in .ini file (geometry,...) in AppData
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.setValue("style", THstyleName);
    settings.setValue("EulerGraphLineColor", EGlineColor.name(QColor::HexArgb));
    settings.setValue("EulerGraphClickByClickMode", EGclickbyClickMode);
    QMainWindow::closeEvent(event);
}

void MainWindow::onAbout()
{
    QMessageBox::about( this, tr("About"),
     tr("Developed in March 2021 by ")+QString("Amir Hammoutene (amir.hammoutene@gmail.com)")+QString(tr("\n\nVersion 1.4.1 - 25 November 2021")) );
}

void MainWindow::onReadme()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("readme.txt"));
}

void MainWindow::onDownloadLink1()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("https://gamejolt.com/games/riseofenigmas/665470"));
}

void MainWindow::onDownloadLink2()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("https://amirhammoutene.itch.io/rise-of-enigmas"));
}

void MainWindow::onSourcesLink()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("https://github.com/AmirHammoutene/RiseOfEnigmas"));
}

void MainWindow::goToHomehPage()
{
    stackedWidget->setCurrentIndex(0);
    // reset modules states, render ...
    eulerGraph->setGraph(0, QPair< QList<Vertex> , QList<Edge> >());
    EGstateLabel->setText("");
}

void MainWindow::applyStyleSheet(QString ssName)
{
    if(ssName == "default")
    {
        qApp->setStyleSheet(QString());
        return;
    }

    QString fileName = ssName+".qss";
    if( !QFile::exists(fileName) )
        return;
    QFile styleFile(fileName);
    if(!styleFile.open(QFile::ReadOnly))
        return;
    QString style( styleFile.readAll() );
    styleFile.close();
    qApp->setStyleSheet(style);
}

void MainWindow::applyDefaultStyle()
{
    THstyleName = "default";
    applyStyleSheet(THstyleName);
    THselectDefaultStyleAction->setChecked(true);
    THselectDarkStyleAction->setChecked(false);
}

void MainWindow::applyDarkStyle()
{
    THstyleName = "dark";
    applyStyleSheet(THstyleName);
    THselectDarkStyleAction->setChecked(true);
    THselectDefaultStyleAction->setChecked(false);
}


void MainWindow::chooseEGlineColor()
{
    QColor colorChosen;
    colorChosen = QColorDialog::getColor(EGlineColor,this,tr("Choose Eulerian Graphs lines' color"),QColorDialog::ShowAlphaChannel);
    if( colorChosen.isValid())
    {
        EGlineColor = colorChosen;
        QColorDialog::setCustomColor(EGcustomColorPosition, EGlineColor);
        EGcustomColorPosition++;
        emit EGlineColorRequest(EGlineColor);
    }
}

void MainWindow::changeEGClickMode(int status)
{
    if(eulerGraph!= nullptr)
    {
        if(status == 0)
        {
            EGclickbyClickMode = false;
            eulerGraph->m_scene.m_clickByClikMode = false;
        }
        else
        {
            EGclickbyClickMode = true;
            eulerGraph->m_scene.m_clickByClikMode = true;
        }
    }
}

void MainWindow::EGStepedUp(uint step, uint total)
{
    if( !EulerGraphStageData.contains( EGscore+1 ))
    {
        EGstateLabel->setText(tr("You finished all levels, good job!"));
        return;
    }
    QString text;
    QString instructionText = tr("You have to complete the graph, crossing each edge only once, without releasing the mouse click.\nLevel ")+QString::number(eulerGraph->m_scene.m_currentStage);
    if(step == 0)
        text = instructionText+" : "+QString::number(total)+tr(" total edges");
    else if( step == total)
        text = tr("Level ")+QString::number(eulerGraph->m_scene.m_currentStage)+tr(". Congratulation, you finished it!");
    else
        text = instructionText+tr(". Good beginning! Edge ")+QString::number(step)+"/"+QString::number(total);
    EGstateLabel->setText(text);
}

void MainWindow::goToEulerGraphPage()
{
    stackedWidget->setCurrentIndex(1);
    if(EulerGraphStageData.contains( EGscore+1 ))
    {
        eulerGraph->setGraph(EGscore+1, EulerGraphStageData.value(EGscore+1) );
        eulerGraph->sceneSendStepUpSignal();
    }
    else
    {
        if(EGtimeChallengeWidget->running)
            EGtimeChallengeWidget->finishChallenge();
        eulerGraph->setGraph(0, QPair< QList<Vertex> , QList<Edge> >());
        EGstateLabel->setText(tr("You finished all levels, good job!"));
    }
}

void MainWindow::setNextEulerGraph()
{
    EGscore = eulerGraph->m_scene.m_currentStage;
    goToEulerGraphPage();
}

void MainWindow::EGstartChallenge()
{
    EGscore = 0;
    eulerGraph->setGraph(0, QPair< QList<Vertex> , QList<Edge> >());
    goToEulerGraphPage();
}

void MainWindow::resetEulerGraphScore()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Reset Eulerian Graphs scores"), tr("Warning, it will reset all levels already completed, are you sure ?"),
                                    QMessageBox::Yes|QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        EGscore = 0;
        eulerGraph->setGraph( EGscore+1, EulerGraphStageData.value(EGscore+1) );
        eulerGraph->sceneSendStepUpSignal();
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // For start/stop Eulerian Graphs Time Challenge (Ctrl+SpaceBar)
    if(stackedWidget->currentIndex() == 1)
    {
        if(event->key() ==  Qt::Key_Space && event->modifiers() == Qt::ControlModifier)
            EGtimeChallengeWidget->startOrStopChrono();
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::constructData()
{
    Vertex O1 = Vertex(0.,0.);
    Vertex A1 = Vertex(-50.,50.);
    Vertex B1 = Vertex(50.,50.);
    Vertex C1 = Vertex(50.,-50.);
    Vertex D1 = Vertex(-50.,-50.);
    Vertex E1 = Vertex(0.,-90.);
    Vertex E3 = Vertex(0.,-140.);
    Vertex A3 = Vertex(-150.,140.);
    Vertex B3 = Vertex(150.,140.);
    Vertex F3 = Vertex(0.,-30.);
    Vertex C3 = Vertex(-40.,50.);
    Vertex D3 = Vertex(40.,50.);
    Vertex A4 = Vertex(-250.,100.);
    Vertex B4 = Vertex(250.,100.);
    Vertex C4 = Vertex(-250.,-100.);
    Vertex D4 = Vertex(250.,-100.);
    Vertex E4 = Vertex(0.,40.);
    Vertex F4 = Vertex(0.,-40.);
    Vertex A5 = Vertex(0.,200.);
    Vertex B5 = Vertex(200.,0.);
    Vertex C5 = Vertex(0.,-200.);
    Vertex D5 = Vertex(-200.,0.);
    Vertex E5 = Vertex(141.,141.);
    Vertex F5 = Vertex(141.,-141.);
    Vertex G5 = Vertex(-141.,-141.);
    Vertex H5 = Vertex(-141.,141.);
    Vertex A6 = Vertex(-270.,210.);
    Vertex B6 = Vertex(-210.,270.);
    Vertex C6 = Vertex(210.,270.);
    Vertex D6 = Vertex(270.,210.);
    Vertex E6 = Vertex(270.,-210.);
    Vertex F6 = Vertex(210.,-270.);
    Vertex G6 = Vertex(-210.,-270.);
    Vertex H6 = Vertex(-270.,-210.);
    Vertex I6 = Vertex(-110.,60.);
    Vertex J6 = Vertex(-60.,110.);
    Vertex K6 = Vertex(60.,110.);
    Vertex L6 = Vertex(110.,60.);
    Vertex M6 = Vertex(110.,-60.);
    Vertex N6 = Vertex(60.,-110.);
    Vertex O6 = Vertex(-60.,-110.);
    Vertex P6 = Vertex(-110.,-60.);
    Vertex A7 = Vertex(-100.,300.);
    Vertex B7 = Vertex(100.,300.);
    Vertex C7 = Vertex(220.,220.);
    Vertex D7 = Vertex(300.,100.);
    Vertex E7 = Vertex(300.,-100.);
    Vertex F7 = Vertex(220.,-220.);
    Vertex G7 = Vertex(100.,-300.);
    Vertex H7 = Vertex(-100.,-300.);
    Vertex I7 = Vertex(-220.,-220.);
    Vertex J7 = Vertex(-300.,-100.);
    Vertex K7 = Vertex(-300.,100.);
    Vertex L7 = Vertex(-220.,220.);
    Vertex M7 = Vertex(0.,170.);
    Vertex N7 = Vertex(100.,150.);
    Vertex O7 = Vertex(150.,100.);
    Vertex P7 = Vertex(170.,0.);
    Vertex Q7 = Vertex(150.,-100.);
    Vertex R7 = Vertex(100.,-150.);
    Vertex S7 = Vertex(0.,-170.);
    Vertex T7 = Vertex(-100.,-150.);
    Vertex U7 = Vertex(-150.,-100.);
    Vertex V7 = Vertex(-170.,0.);
    Vertex W7 = Vertex(-150.,100.);
    Vertex X7 = Vertex(-100.,150.);
    Vertex A8 = Vertex(-100.,250.);
    Vertex B8 = Vertex(100.,250.);
    Vertex C8 = Vertex(250.,100.);
    Vertex D8 = Vertex(250.,-100.);
    Vertex E8 = Vertex(100.,-250.);
    Vertex F8 = Vertex(-100.,-250.);
    Vertex G8 = Vertex(-250.,-100.);
    Vertex H8 = Vertex(-250.,100.);
    Vertex I8 = Vertex(-40.,100.);
    Vertex J8 = Vertex(40.,100.);
    Vertex K8 = Vertex(100.,40.);
    Vertex L8 = Vertex(100.,-40.);
    Vertex M8 = Vertex(40.,-100.);
    Vertex N8 = Vertex(-40.,-100.);
    Vertex O8 = Vertex(-100.,-40.);
    Vertex P8 = Vertex(-100.,40.);

    EulerGraphStageData.insert
        ( 1 , QPair< QList<Vertex> , QList<Edge> >( {O1,C1,D1,E1}  ,
          { Edge(C1,D1),Edge(C1,E1),
          Edge(D1,O1),Edge(D1,E1) } )
          ) ;
    EulerGraphStageData.insert
        ( 2 , QPair< QList<Vertex> , QList<Edge> >( {O1,A1,B1,C1,D1,E1}  ,
          { Edge(A1,B1),Edge(A1,O1),Edge(A1,D1),
          Edge(B1,O1),Edge(B1,C1),
           Edge(C1,O1),Edge(C1,D1),Edge(C1,E1),
          Edge(D1,O1),Edge(D1,E1) } )
          ) ;
    EulerGraphStageData.insert
        ( 3 , QPair< QList<Vertex> , QList<Edge> >( {E3,A3,B3,C3,D3,F3}  ,
          { Edge(E3,A3), Edge(E3,B3), Edge(E3,C3),Edge(E3,D3),
            Edge(A3,B3), Edge(A3,D3), Edge(A3,F3),
            Edge(B3,C3), Edge(B3,F3),
            Edge(C3,D3), Edge(D3,F3), Edge(F3,C3)} )
          ) ;
    EulerGraphStageData.insert
        ( 4 , QPair< QList<Vertex> , QList<Edge> >( {A4,B4,C4,D4,E4,F4}  ,
          { Edge(A4,B4),Edge(A4,D4),Edge(A4,E4),Edge(A4,F4),
            Edge(B4,C4),Edge(B4,E4),Edge(B4,F4),
            Edge(C4,D4),Edge(C4,E4),Edge(C4,F4),
            Edge(D4,E4),Edge(D4,F4)} )
          ) ;
    EulerGraphStageData.insert
        ( 5 , QPair< QList<Vertex> , QList<Edge> >( {A5,B5,C5,D5,E5,F5,G5,H5}  ,
          { Edge(A5,E5),Edge(A5,F5),Edge(A5,C5),Edge(A5,G5),Edge(A5,D5),Edge(A5,H5),
            Edge(B5,E5),Edge(B5,F5),Edge(B5,C5),Edge(B5,G5),Edge(B5,D5),Edge(B5,H5),
            Edge(C5,E5),Edge(C5,F5),Edge(C5,G5),Edge(C5,H5),
            Edge(D5,H5),Edge(D5,E5),Edge(D5,F5),Edge(D5,G5),
            Edge(E5,F5),Edge(E5,G5),
            Edge(F5,H5),Edge(G5,H5)} )
          ) ;
    EulerGraphStageData.insert
        ( 6 , QPair< QList<Vertex> , QList<Edge> >( {A6,B6,C6,D6,E6,F6,G6,H6,I6,J6,K6,L6,M6,N6,O6,P6}  ,
          { Edge(A6,B6),Edge(A6,D6),Edge(A6,I6),Edge(A6,H6),
            Edge(B6,C6),Edge(B6,J6),Edge(B6,G6),
            Edge(C6,D6),Edge(C6,F6),Edge(C6,K6),
            Edge(D6,E6),Edge(D6,L6),Edge(E6,F6),Edge(E6,H6),Edge(E6,M6),
            Edge(F6,G6),Edge(F6,N6),Edge(G6,H6),Edge(G6,O6),Edge(H6,P6),
            Edge(I6,J6),Edge(I6,P6),Edge(I6,L6),
            Edge(J6,K6),Edge(J6,O6),Edge(K6,L6),Edge(K6,N6),
            Edge(L6,M6),Edge(M6,N6),Edge(M6,P6),Edge(N6,O6),Edge(O6,P6)} )
          ) ;
    EulerGraphStageData.insert
        ( 7 , QPair< QList<Vertex> , QList<Edge> >( {A8,B8,C8,D8,E8,F8,G8,H8,I8,J8,K8,L8,M8,N8,O8,P8}  ,
          { Edge(A8,B8),Edge(A8,J8),Edge(A8,P8),Edge(A8,H8),
            Edge(B8,I8),Edge(B8,K8),Edge(B8,C8),Edge(C8,J8),Edge(C8,L8),Edge(C8,D8),
            Edge(D8,K8),Edge(D8,M8),Edge(D8,E8),Edge(E8,L8),Edge(E8,N8),Edge(E8,F8),
            Edge(F8,M8),Edge(F8,O8),Edge(F8,G8),Edge(G8,N8),Edge(G8,P8),Edge(G8,H8),
            Edge(H8,O8),Edge(H8,I8),Edge(I8,L8),Edge(I8,N8),
            Edge(J8,M8),Edge(J8,O8),Edge(K8,P8),Edge(K8,N8),Edge(L8,O8),Edge(M8,P8) } )
          ) ;
    EulerGraphStageData.insert
        ( 8 , QPair< QList<Vertex> , QList<Edge> >( {A7,B7,C7,D7,E7,F7,G7,H7,I7,J7,K7,L7,M7,N7,O7,P7,Q7,R7,S7,T7,U7,V7,W7,X7}  ,
          { Edge(A7,B7),Edge(A7,C7),Edge(A7,N7),Edge(A7,M7),Edge(A7,X7),Edge(A7,W7),Edge(A7,K7),Edge(A7,L7),
            Edge(B7,C7),Edge(B7,D7),Edge(B7,O7),Edge(B7,N7),Edge(B7,M7),Edge(B7,X7),Edge(B7,L7),
            Edge(C7,D7),Edge(C7,E7),Edge(C7,M7),Edge(C7,N7),Edge(C7,O7),Edge(C7,P7),
            Edge(D7,E7),Edge(D7,F7),Edge(D7,N7),Edge(D7,O7),Edge(D7,P7),Edge(D7,Q7),
            Edge(E7,F7),Edge(E7,G7),Edge(E7,O7),Edge(E7,P7),Edge(E7,Q7),Edge(E7,R7),
            Edge(F7,G7),Edge(F7,H7),Edge(F7,P7),Edge(F7,Q7),Edge(F7,R7),Edge(F7,S7),
            Edge(G7,H7),Edge(G7,I7),Edge(G7,Q7),Edge(G7,R7),Edge(G7,S7),Edge(G7,T7),
            Edge(H7,I7),Edge(H7,J7),Edge(H7,R7),Edge(H7,S7),Edge(H7,T7),Edge(H7,U7),
            Edge(I7,J7),Edge(I7,K7),Edge(I7,S7),Edge(I7,T7),Edge(I7,U7),Edge(I7,V7),
            Edge(J7,K7),Edge(J7,L7),Edge(J7,T7),Edge(J7,U7),Edge(J7,V7),Edge(J7,W7),
            Edge(K7,L7),Edge(K7,U7),Edge(K7,V7),Edge(K7,W7),Edge(K7,X7),
            Edge(L7,V7),Edge(L7,W7),Edge(L7,X7),Edge(L7,M7),
            Edge(M7,N7),Edge(M7,O7),Edge(M7,R7),Edge(M7,T7),Edge(M7,W7),Edge(M7,X7),
            Edge(N7,O7),Edge(N7,P7),Edge(N7,S7),Edge(N7,U7),Edge(N7,X7),
            Edge(O7,P7),Edge(O7,Q7),Edge(O7,T7),Edge(O7,V7),
            Edge(P7,Q7),Edge(P7,R7),Edge(P7,U7),Edge(P7,W7),
            Edge(Q7,R7),Edge(Q7,S7),Edge(Q7,V7),Edge(Q7,X7),
            Edge(R7,S7),Edge(R7,T7),Edge(R7,W7),
            Edge(S7,T7),Edge(S7,U7),Edge(S7,X7),
            Edge(T7,U7),Edge(T7,V7),Edge(U7,V7),Edge(U7,W7),
            Edge(V7,W7),Edge(V7,X7),Edge(W7,X7)} )
          ) ;
}
