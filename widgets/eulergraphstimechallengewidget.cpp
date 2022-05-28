#include "eulergraphstimechallengewidget.h"
#include "ui_eulergraphstimechallengewidget.h"

EulerGraphsTimeChallengeWidget::EulerGraphsTimeChallengeWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::EulerGraphsTimeChallengeWidget)
{
    ui->setupUi(this);
    setFixedHeight(100);
    setFrameShape(QFrame::Box);
    setFrameShadow(QFrame::Raised);

    ui->sendScoreButton->setStyleSheet("QPushButton {color:orange;}");
    ui->sendScoreButton->setVisible(false);

    running = false;
    timer.setInterval(100);

    sendScoreDialog = new EulerGraphsTimeChallengeSendScoreDialog(this);
    recordsDialog = new EulerGraphsTimeChallengeRecordsDialog(this);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(ui->startButton, SIGNAL(released()), this, SLOT(startOrStopChrono()));
    QObject::connect(ui->sendScoreButton, SIGNAL(released()), this, SLOT(showSendScoreDialog()));
    QObject::connect(ui->recordsButton, SIGNAL(released()), recordsDialog, SLOT(showAndLoad()));
    QObject::connect(ui->goToChallengePageButton, SIGNAL(released()), this, SLOT(goToChallengePage()));
}

EulerGraphsTimeChallengeWidget::~EulerGraphsTimeChallengeWidget()
{
    delete ui;
}

void EulerGraphsTimeChallengeWidget::finishChallenge()
{
    startOrStopChrono();
    ui->sendScoreButton->setVisible(true);
}

void EulerGraphsTimeChallengeWidget::scaleToScreen()
{
    setFixedHeight(sizeHint().height() * property("heightFactor").toReal());
    ui->eulerGraphsTimeChallengeStackedWidget->setFixedHeight(
                ui->eulerGraphsTimeChallengeStackedWidget->sizeHint().height()
                * property("heightFactor").toReal());
    QFont base12Font = ui->label_3->font();
    base12Font.setPointSize(base12Font.pointSize() * property("heightFactor").toReal());
    ui->label_3->setFont(base12Font);
    ui->label->setFont(base12Font);
    QFont base14Font = ui->goToChallengePageButton->font();
    base14Font.setPointSize(base14Font.pointSize() * property("heightFactor").toReal());
    ui->goToChallengePageButton->setFont(base14Font);
    QFont startFont = ui->startButton->font();
    startFont.setPointSize(startFont.pointSize() * property("heightFactor").toReal());
    ui->startButton->setFont(startFont);
    QFont otherButtonFont = ui->sendScoreButton->font();
    otherButtonFont.setPointSize(otherButtonFont.pointSize() * property("heightFactor").toReal());
    ui->sendScoreButton->setFont(otherButtonFont);
    ui->recordsButton->setFont(otherButtonFont);
    QFont timeFont = ui->timeElapsedLabel->font();
    timeFont.setPointSize(timeFont.pointSize() * property("heightFactor").toReal());
    ui->timeElapsedLabel->setFont(timeFont);
}

void EulerGraphsTimeChallengeWidget::updateTime()
{
    if(!running || !elapsedTimer.isValid())
    {
        ui->timeElapsedLabel->setText("");
        return;
    }

    qint64 time = elapsedTimer.elapsed();
    int tenth = (time/100) % 10;
    int seconds = time/1000;
    ui->timeElapsedLabel->setText(QString::number(seconds)+QString(".")+QString::number(tenth));
}

void EulerGraphsTimeChallengeWidget::startOrStopChrono()
{
    if(ui->eulerGraphsTimeChallengeStackedWidget->currentIndex() != 1)
        return;
    if(!running)
    {
        emit startChallengeRequest();
        running = true;
        elapsedTimer.start();
        timer.start();
        ui->startButton->setText(tr("Stop Challenge"));
        ui->startButton->setStyleSheet("QPushButton {color: red;}");
        ui->sendScoreButton->setVisible(false);
    }
    else
    {
        timer.stop();
        elapsedTimer.invalidate();
        running = false;
        ui->startButton->setText(tr("Start Challenge"));
        ui->startButton->setStyleSheet("QPushButton {}");
    }
}

void EulerGraphsTimeChallengeWidget::showSendScoreDialog()
{
    sendScoreDialog->setTimeScore(ui->timeElapsedLabel->text());
    sendScoreDialog->show();
}

void EulerGraphsTimeChallengeWidget::goToChallengePage()
{
    ui->eulerGraphsTimeChallengeStackedWidget->setCurrentIndex(1);
}

void EulerGraphsTimeChallengeWidget::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
       ui->retranslateUi(this);
    }
    QFrame::changeEvent(event);
}
