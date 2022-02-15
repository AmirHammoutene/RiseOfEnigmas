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

void EulerGraphsInfoAndOptionsWidget::scaleToScreen()
{
    ui->EGchangeLineColorButton->setFixedHeight(ui->EGchangeLineColorButton->sizeHint().height()
                                                * property("heightFactor").toReal());
    QFont labelAndCheckBoxFont = ui->EGstateLabel->font();
    labelAndCheckBoxFont.setPointSize(labelAndCheckBoxFont.pointSize()* property("heightFactor").toReal());
    ui->EGstateLabel->setFont(labelAndCheckBoxFont);
    ui->EGeasyMode->setFont(labelAndCheckBoxFont);
    QFont buttonsFont = ui->EGchangeLineColorButton->font();
    buttonsFont.setPointSize(buttonsFont.pointSize()* property("heightFactor").toReal());
    ui->EGchangeLineColorButton->setFont(buttonsFont);
    ui->EGresetButton->setFont(buttonsFont);
    ui->EGhomeButton->setFont(buttonsFont);
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
