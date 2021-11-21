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

    ui->sendScoreButton->setStyleSheet("QPushButton {color:green;}");
    ui->sendScoreButton->setVisible(false);

    running = false;
    timer.setInterval(100);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    QObject::connect(ui->startButton, SIGNAL(released()), this, SLOT(startOrStopChrono()));
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
    if(!running)
    {
        emit startChallengeRequest();
        running = true;
        elapsedTimer.start();
        timer.start();
        ui->startButton->setText("Stop Challenge");
        ui->startButton->setStyleSheet("QPushButton {color: red;}");
        ui->sendScoreButton->setVisible(false);
    }
    else
    {
        timer.stop();
        elapsedTimer.invalidate();
        running = false;
        ui->startButton->setText("Start Challenge");
        ui->startButton->setStyleSheet("QPushButton {}");
    }
}