#include "eulergraphgenerationpage.h"
#include "mainwindow.h"
#include "eulergraphspage.h"
#include "workers/euleriangraphgenerationworker.h"
#include <QSettings>
#include <QStandardPaths>
#include <QGridLayout>
#include <QColorDialog>
#include <QInputDialog>
#include <QThread>

const int EulerGraphGenerationPage::minimumVerticesNumber = 10;
const int EulerGraphGenerationPage::maximumVerticesNumber = 80;

EulerGraphGenerationPage::EulerGraphGenerationPage(QWidget *parent)
    : QScrollArea{parent}
{
    m_parent = qobject_cast<MainWindow *>(parent);
    if(!m_parent)
        return;

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::defaultFormat(), QSettings::UserScope,
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)); // Most settings stored in a ini file in AppData
    QSettings settings;
    finishCreate();

    m_isValid = true;
}

void EulerGraphGenerationPage::scaleToScreen()
{
    QFont normalBtnFont = normalPageButton->font();
    normalBtnFont.setPointSize(16*property("heightFactor").toReal());
    normalPageButton->setFont(normalBtnFont);
    normalPageButton->setFixedSize(300*property("widthFactor").toReal(),35*property("heightFactor").toReal());

    QFont generateBtnFont = generateButton->font();
    generateBtnFont.setPointSize(25*property("heightFactor").toReal());
    generateButton->setFont(generateBtnFont);
    generateButton->setFixedSize(300*property("widthFactor").toReal(),60*property("heightFactor").toReal());

    eulerGraph->setProperty("heightFactor",property("heightFactor").toReal());
    eulerGraph->scaleToScreen();

    EGinfoOptionsWidget->setProperty("heightFactor",property("heightFactor").toReal());
    EGinfoOptionsWidget->scaleToScreen();
}

void EulerGraphGenerationPage::resetPage()
{
    eulerGraph->setGraph(0, QPair< QList<Vertex> , QList<Edge> >());
    EGinfoOptionsWidget->setInstructionsText("");
    if(!congradulationPopup->isHidden())
        congradulationPopup->close();
}

void EulerGraphGenerationPage::inheritEulerGraphsPageSettings(EulerGraphsPage *eulerGraphsPage)
{
    if(!eulerGraphsPage || !eulerGraphsPage->m_isValid)
        return;

    m_eulerGraphsPage = eulerGraphsPage;

    EGcustomColorPosition = m_eulerGraphsPage->EGcustomColorPosition;
    EGlineColor = m_eulerGraphsPage->EGlineColor;
    QColorDialog::setCustomColor(0, EGlineColor);
    emit EGlineColorRequest(EGlineColor);
    EGclickbyClickMode = m_eulerGraphsPage->EGclickbyClickMode;
    EGinfoOptionsWidget->setMagnetModeChecked(!EGclickbyClickMode);
    changeEGMagnetMode(!EGclickbyClickMode);
}

void EulerGraphGenerationPage::finishCreate()
{
    eulerGraph = new EulerGraphInteract(this);

    EGinfoOptionsWidget = new EulerGraphsInfoAndOptionsWidget(this);
    EGinfoOptionsWidget->setResetScoreVisible(false);

    normalPageButton = new QPushButton(tr("Return to normal mode"),this);

    generateButton = new QPushButton(tr("Generate a Graph..."),this);

    QGridLayout *EGmainLayout = new QGridLayout(this);
    EGmainLayout->addItem(new QSpacerItem(10,10),0,0);
    EGmainLayout->addWidget(normalPageButton, 0, 1,Qt::AlignRight);
    EGmainLayout->addItem(new QSpacerItem(10,10),0,2);
    EGmainLayout->addWidget(eulerGraph,1,1);
    EGmainLayout->addWidget(generateButton,2,1,Qt::AlignCenter);
    EGmainLayout->addWidget(EGinfoOptionsWidget,3,1);
    EGmainLayout->addItem(new QSpacerItem(10,10),4,1);
    this->setLayout(EGmainLayout);

    progressDialog = new GraphGenerationProgressDialog(this);
    progressDialog->hide();

    QObject::connect( &eulerGraph->m_scene, SIGNAL( stepedUp(uint,uint) ), this, SLOT( EGStepedUp(uint,uint) ) );
    QObject::connect( eulerGraph, SIGNAL( fullFinishedGraph() ), this, SLOT( graphFinished() ) );
    QObject::connect( EGinfoOptionsWidget, SIGNAL(chooseEGlineColorRequest()), this, SLOT(chooseEGlineColor()));
    QObject::connect( EGinfoOptionsWidget, SIGNAL( homePageRequest() ), m_parent, SLOT( goToHomehPage() ) );
    QObject::connect( EGinfoOptionsWidget, SIGNAL( magnetModeStateChanged(int) ), this, SLOT( changeEGMagnetMode(int) ) );;
    QObject::connect( EGinfoOptionsWidget, SIGNAL(translateEGInstructionsRequest()), this, SLOT(translateEGInstructions()));
    QObject::connect( this, SIGNAL( EGlineColorRequest(QColor)) , &eulerGraph->m_scene, SLOT( changeLineColor(QColor) ) ) ;
    QObject::connect( normalPageButton, SIGNAL( released()) , this, SLOT( goToNormalPage() ) ) ;
    QObject::connect( generateButton, SIGNAL( released()) , this, SLOT( getGenerationEdgesNumber() ) ) ;
    QObject::connect( progressDialog, SIGNAL( rejected()) , this, SLOT( generationCancelled() ) ) ;

    congradulationPopup = new CongradulationDialog(this);
}

void EulerGraphGenerationPage::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        normalPageButton->setText(tr("Return to normal mode"));
        generateButton->setText(tr("Generate a Graph..."));
        translateEGInstructions();
    }
    QScrollArea::changeEvent(event);
}

void EulerGraphGenerationPage::chooseEGlineColor()
{

    if(m_parent->stackedWidget->currentIndex() != 2)
        return;

    QColor colorChosen;
    colorChosen = QColorDialog::getColor(EGlineColor,this,tr("Choose Eulerian Graphs lines' color"),QColorDialog::ShowAlphaChannel);
    if( colorChosen.isValid())
    {
        EGlineColor = colorChosen;
        QColorDialog::setCustomColor(EGcustomColorPosition, EGlineColor);
        EGcustomColorPosition++;
        emit EGlineColorRequest(EGlineColor);
        m_eulerGraphsPage->inheritEulerGraphGenerationPageSettings(this);
    }
}

void EulerGraphGenerationPage::changeEGMagnetMode(int status)
{
    if(eulerGraph!= nullptr)
    {
        if(status == 0)
        {
            EGclickbyClickMode = true;
            eulerGraph->m_scene.m_clickByClikMode = true;
        }
        else
        {
            EGclickbyClickMode = false;
            eulerGraph->m_scene.m_clickByClikMode = false;
        }
    }

    m_eulerGraphsPage->inheritEulerGraphGenerationPageSettings(this);
}

void EulerGraphGenerationPage::EGStepedUp(uint step, uint total)
{
    QString text;
    QString instructionText = tr("You have to complete the graph by passing through each of the edges only once, without lifting pencil.\n");
    if(step == 0)
        text = instructionText+tr("Generated level : ")+QString::number(total)+tr(" total edges");
    else if( step == total)
        text = tr("Congratulation, you finished it!");
    else
        text = instructionText+tr("Generated Level. Good beginning! Edge ")+QString::number(step)+"/"+QString::number(total);
    EGinfoOptionsWidget->setInstructionsText(text);
}

void EulerGraphGenerationPage::translateEGInstructions()
{
    if(m_parent->stackedWidget->currentIndex() == 2)
        eulerGraph->sceneSendStepUpSignal();
}

void EulerGraphGenerationPage::graphFinished()
{
    eulerGraph->setGraph(0, QPair< QList<Vertex> , QList<Edge> >());
    congradulationPopup->show();
}

void EulerGraphGenerationPage::goToNormalPage()
{
    if(m_parent->EGmainWidget->m_isValid)
    {
        resetPage();
        m_parent->stackedWidget->setCurrentIndex(1);
        m_eulerGraphsPage->inheritEulerGraphGenerationPageSettings(this);
        m_eulerGraphsPage->refreshShow();
    }
}

void EulerGraphGenerationPage::getGenerationEdgesNumber()
{
    bool ok;
    int verticesNumRequested = QInputDialog::getInt(this, tr("Number of vertices required"),
                                 tr("How many vertices would you like to generate in your Eulerian Graph?"), minimumVerticesNumber, minimumVerticesNumber, maximumVerticesNumber, 10, &ok);
    if(!ok)
        return;

    resetPage();

    QThread* thread = new QThread();
    EulerianGraphGenerationWorker* worker = new EulerianGraphGenerationWorker();
    worker->m_verticesNumberRequested = verticesNumRequested >=minimumVerticesNumber && verticesNumRequested<=maximumVerticesNumber ? verticesNumRequested : minimumVerticesNumber;
    worker->moveToThread(thread);

    connect( worker, &EulerianGraphGenerationWorker::sendStepInfo, progressDialog, &GraphGenerationProgressDialog::setStepTextAndProgressValue);
    connect( worker, &EulerianGraphGenerationWorker::sendGraphData, this, &EulerGraphGenerationPage::generatedGraphDataReceived);
    connect( thread, &QThread::started, worker, &EulerianGraphGenerationWorker::process);
    connect( this, &EulerGraphGenerationPage::cancelGeneration, thread, &QThread::quit, Qt::DirectConnection);
    connect( this, &EulerGraphGenerationPage::cancelGeneration, worker, &EulerianGraphGenerationWorker::deleteLater, Qt::DirectConnection);
    connect( worker, &EulerianGraphGenerationWorker::finished, thread, &QThread::quit);
    connect( worker, &EulerianGraphGenerationWorker::finished, worker, &EulerianGraphGenerationWorker::deleteLater);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();

    progressDialog->show();
}

void EulerGraphGenerationPage::generatedGraphDataReceived(QPair<QList<Vertex>, QList<Edge> > data)
{
    eulerGraph->setGraph(1, data );
    eulerGraph->sceneSendStepUpSignal();
    progressDialog->hide();
}

void EulerGraphGenerationPage::generationCancelled()
{
    emit cancelGeneration();
}
