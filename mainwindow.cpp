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
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Rise Of Enigmas");

    translator = new QTranslator();
    baseTranslator = new QTranslator();

    // Load window settings from .ini file (geometry,...)
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::defaultFormat(), QSettings::UserScope,
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)); // Most settings stored in a ini file in AppData
    QSettings settings;

    showFullScreen();
    finishCreate();

    applyStyleSheet();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scaleToScreen()
{
    HPEulerGraphButton->setIconSize(QSize(210 * property("widthFactor").toReal(), 243 * property("heightFactor").toReal()));
    HPcomingSoonButton->setIconSize(QSize(210 * property("widthFactor").toReal(), 243 * property("heightFactor").toReal()));

    QFont labelsFont = HPEulerGraphLabel->font();
    labelsFont.setPointSize(labelsFont.pointSize()* property("heightFactor").toReal());
    HPEulerGraphLabel->setFont(labelsFont);
    HPcomingSoonLabel->setFont(labelsFont);

    musicPlayerWidget->setProperty("heightFactor",property("heightFactor").toReal());
    musicPlayerWidget->setProperty("widthFactor",property("widthFactor").toReal());
    musicPlayerWidget->scaleToScreen();

    if(EGmainWidget->m_isValid)
    {
        EGmainWidget->setProperty("heightFactor",property("heightFactor").toReal());
        EGmainWidget->setProperty("widthFactor",property("widthFactor").toReal());
        EGmainWidget->scaleToScreen();
    }

    if(EGgenerationWidget->m_isValid)
    {
        EGgenerationWidget->setProperty("heightFactor",property("heightFactor").toReal());
        EGgenerationWidget->setProperty("widthFactor",property("widthFactor").toReal());
        EGgenerationWidget->scaleToScreen();
    }

    menuDialog->setProperty("heightFactor",property("heightFactor").toReal());
    menuDialog->scaleToScreen();
}

void MainWindow::finishCreate()
{
    statusBar()->hide();
    menuBar()->hide();

    musicPlayerWidget = new MusicPlayerWidget(this);

    stackedWidget = new QStackedWidget(centralWidget());

    /// HOME PAGE

    QScrollArea *HPmainWidget = new QScrollArea(stackedWidget);
    QGridLayout *HPmainLayout = new QGridLayout(HPmainWidget);

    HPEulerGraphButton = new QPushButton(QIcon(":/img/EulerGraph.png"),"",HPmainWidget);
    HPEulerGraphButton->setIconSize(QSize(210 , 243));
    HPEulerGraphButton->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    HPEulerGraphLabel = new QLabel(tr("Eulerian Graphs"),HPmainWidget);
    HPEulerGraphLabel->setFont(QFont("Helvetica", 14,75));
    HPEulerGraphLabel->setAlignment(Qt::AlignCenter);
    HPEulerGraphLabel->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);

    QVBoxLayout *HPEulerGraphLayout = new QVBoxLayout();
    HPEulerGraphLayout->addWidget(HPEulerGraphButton);
    HPEulerGraphLayout->addSpacing(30);
    HPEulerGraphLayout->addWidget(HPEulerGraphLabel);

    HPcomingSoonButton = new QPushButton(QIcon(":/img/comingSoon.png"),"",HPmainWidget);
    HPcomingSoonButton->setIconSize(QSize(210,243));
    HPcomingSoonButton->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);
    HPcomingSoonButton->setEnabled(false);
    HPcomingSoonLabel = new QLabel(tr("coming soon..."),HPmainWidget);
    HPcomingSoonLabel->setFont(QFont("Helvetica", 14,75));
    HPcomingSoonLabel->setAlignment(Qt::AlignCenter);
    HPcomingSoonLabel->setSizePolicy(QSizePolicy::Policy::Minimum,QSizePolicy::Policy::Minimum);

    QVBoxLayout *HPcomingSoonLayout = new QVBoxLayout();
    HPcomingSoonLayout->addWidget(HPcomingSoonButton);
    HPcomingSoonLayout->addSpacing(30);
    HPcomingSoonLayout->addWidget(HPcomingSoonLabel);


    HPmainLayout->addLayout(HPEulerGraphLayout,1,1);
    HPmainLayout->addItem(new QSpacerItem(40,40),1,2);
    HPmainLayout->addLayout(HPcomingSoonLayout,1,3);
    HPmainLayout->setColumnStretch(0,1);
    HPmainLayout->setColumnStretch(4,1);
    HPmainLayout->setRowStretch(0,1);
    HPmainLayout->setRowStretch(2,1);

    QObject::connect(HPEulerGraphButton, SIGNAL(released()), this, SLOT(goToEulerGraphPage()));

    stackedWidget->addWidget(HPmainWidget);

    /// EULERIAN GRAPHS PAGE
    EGmainWidget = new EulerGraphsPage(this);
    EGgenerationWidget = new EulerGraphGenerationPage(this);

    ///

    if(EGmainWidget->m_isValid)
        stackedWidget->addWidget(EGmainWidget);

    if(EGgenerationWidget->m_isValid)
        stackedWidget->addWidget(EGgenerationWidget);

    stackedWidget->setCurrentIndex(0);

    QVBoxLayout *mainWindowLayout = new QVBoxLayout();
    mainWindowLayout->addWidget(musicPlayerWidget);
    mainWindowLayout->addWidget(stackedWidget);

    musicPlayerWidget->setVisible(false);

    centralWidget()->setLayout(mainWindowLayout);

    menuDialog = new MenuDialog(this);
    QObject::connect( menuDialog, SIGNAL(changeToEnglishLanguage()), this, SLOT( changeToEnglishLanguage() ) );
    QObject::connect( menuDialog, SIGNAL(changeToFrenchLanguage()), this, SLOT( changeToFrenchLanguage() ) );
    QObject::connect( menuDialog, SIGNAL(openAbout()), this, SLOT( onAbout() ) );
    QObject::connect( menuDialog, SIGNAL(openSourcesLink()), this, SLOT( onSourcesLink() ) );
    QObject::connect( menuDialog, SIGNAL(quitApp()), this, SLOT( close() ) );
    if(EGmainWidget->m_isValid)
    {
        QObject::connect( menuDialog, SIGNAL(EGopenLineColorSelection()), EGmainWidget, SLOT( chooseEGlineColor() ) );
        QObject::connect( menuDialog, SIGNAL(EGresetLevels()), EGmainWidget, SLOT( resetEulerGraphScore() ) );
    }

    if(EGgenerationWidget->m_isValid)
    {
        QObject::connect( menuDialog, SIGNAL(EGopenLineColorSelection()), EGgenerationWidget, SLOT( chooseEGlineColor() ) );
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(EGmainWidget->m_isValid)
    {
        QSettings scores(QSettings::UserScope, "Amir Hammoutene", "Rise Of Enigmas");
        scores.setValue("EulerGraphScore", EGmainWidget->EGscore);
        QSettings settings;
        settings.setValue("EulerGraphLineColor", EGmainWidget->EGlineColor.name(QColor::HexArgb));
        settings.setValue("EulerGraphClickByClickMode", EGmainWidget->EGclickbyClickMode);
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::onAbout()
{
    QMessageBox::about( this, tr("About"),
     tr("Developed in March 2021 by ")+QString("Amir Hammoutene (amir.hammoutene@gmail.com)")+QString(tr("\n\nMusic by Yeo Sky"))
                        +QString(tr("\n\nVersion Steam 2.0.0 - 28 May 2022")) );
}

void MainWindow::onSourcesLink()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile("https://github.com/AmirHammoutene/RiseOfEnigmas"));
}

void MainWindow::goToHomehPage()
{
    stackedWidget->setCurrentIndex(0);
    if(EGmainWidget->m_isValid)
        EGmainWidget->resetPage();
    if(EGgenerationWidget->m_isValid)
        EGgenerationWidget->resetPage();
    musicPlayerWidget->stopMusic();
    musicPlayerWidget->setVisible(false);

}

void MainWindow::applyStyleSheet()
{
    QString fileName = "dark.qss";
    if( !QFile::exists(fileName) )
        return;
    QFile styleFile(fileName);
    if(!styleFile.open(QFile::ReadOnly))
        return;
    QString style( styleFile.readAll() );
    styleFile.close();
    qApp->setStyleSheet(style);
}

void MainWindow::goToEulerGraphPage()
{
    if(EGmainWidget->m_isValid)
    {
        musicPlayerWidget->setVisible(true);
        stackedWidget->setCurrentIndex(1);
        EGmainWidget->refreshShow();

        if(!musicPlayerWidget->dontDisturbPlaying)
        {
            musicPlayerWidget->setMusicAndPlayLoop("music/YeoSky_Enigma.wav");
            musicPlayerWidget->setMusicTitle("Yeo Sky - Enigma");
        }
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //Menu
    if(event->key() ==  Qt::Key_Escape)
    {
        if(menuDialog->isHidden())
            menuDialog->show();
        else
            menuDialog->hide();
    }
    if(EGmainWidget->m_isValid)
    {
        // For start/stop Eulerian Graphs Time Challenge (Ctrl+SpaceBar)
        if(stackedWidget->currentIndex() == 1)
        {
            if(event->key() ==  Qt::Key_Space && event->modifiers() == Qt::ControlModifier)
                EGmainWidget->EGtimeChallengeWidget->startOrStopChrono();
        }
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
       ui->retranslateUi(this);
       HPEulerGraphLabel->setText(tr("Eulerian Graphs"));
       HPcomingSoonLabel->setText(tr("coming soon..."));
    }
    QMainWindow::changeEvent(event);
}


void MainWindow::changeToEnglishLanguage()
{
    if(translator->load(QString("RiseOfEnigmas_en")))
        qApp->installTranslator(translator);

    if(baseTranslator->load(QString("qtbase_en")))
        qApp->installTranslator(baseTranslator);
}

void MainWindow::changeToFrenchLanguage()
{
    if(translator->load(QString("RiseOfEnigmas_fr")))
        qApp->installTranslator(translator);

    if(baseTranslator->load(QString("qtbase_fr")))
        qApp->installTranslator(baseTranslator);
}
