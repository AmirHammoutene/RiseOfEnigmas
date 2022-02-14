#include "eulergraphsinfoandoptionswidget.h"
#include "ui_eulergraphsinfoandoptionswidget.h"

EulerGraphsInfoAndOptionsWidget::EulerGraphsInfoAndOptionsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EulerGraphsInfoAndOptionsWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->EGeasyMode, SIGNAL( stateChanged(int) ), this, SIGNAL( easyModeStateChanged(int) ) );
    QObject::connect(ui->EGchangeLineColorButton, SIGNAL(released()), this, SIGNAL(chooseEGlineColorRequest()));
    QObject::connect(ui->EGhomeButton, SIGNAL(released()), this, SIGNAL(homePageRequest()));
    QObject::connect(ui->EGresetButton, SIGNAL(released()), this, SIGNAL(resetEulerGraphScoreRequest()));
}

EulerGraphsInfoAndOptionsWidget::~EulerGraphsInfoAndOptionsWidget()
{
    delete ui;
}

void EulerGraphsInfoAndOptionsWidget::setClickByClickModeChecked(bool val)
{
    ui->EGeasyMode->setChecked(val);
}

void EulerGraphsInfoAndOptionsWidget::setInstructionsText(const QString &str)
{
    ui->EGstateLabel->setText(str);
}

void EulerGraphsInfoAndOptionsWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
       ui->retranslateUi(this);
       emit translateInstructionsRequest();
    }
    QWidget::changeEvent(event);
}
