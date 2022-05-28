#include "musicplayerwidget.h"
#include "ui_musicplayerwidget.h"
#include <QMediaDevices>
#include <QDebug>
#include <QFile>

const int MusicPlayerWidget::DefaultVolume = 5;
bool MusicPlayerWidget::hasManuallyChosenDevice = false;
int MusicPlayerWidget::chosenDeviceIndex = 0;
bool MusicPlayerWidget::dontDisturbPlaying = false;
bool MusicPlayerWidget::isOnPause = false;

MusicPlayerWidget::MusicPlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayerWidget)
{
    ui->setupUi(this);
    musicPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput();
    device = new QAudioDevice();
    audioOutput->setVolume((float)(DefaultVolume/100.));
    ui->volumeSlider->setValue(DefaultVolume);
    musicPlayer->setAudioOutput(audioOutput);
    ui->pushButton->setFixedSize(42,42);
    ui->pushButton->setIcon(QIcon(":/img/pause.png"));

    QObject::connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentDeviceChanged(int)));
    QObject::connect(ui->positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderPositionChanged(int)));
    QObject::connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderVolumeChanged(int)));
    QObject::connect(ui->pushButton, SIGNAL(released()), this, SLOT(onResumeOrPause()));
}

MusicPlayerWidget::~MusicPlayerWidget()
{
    delete ui;
}

void MusicPlayerWidget::setMusicAndPlayLoop(QString sourceStr)
{
    ui->comboBox->clear();
    auto devices = QMediaDevices::audioOutputs();
    if(devices.size() == 0)
        return;
    int defaultIdx = 0;
    for (int i = 0; i < devices.size(); ++i)
    {
        auto ad = devices.at(i);
        ui->comboBox->addItem(ad.description());
        if(ad.isDefault())
        {
            device = new QAudioDevice(ad);
            audioOutput->setDevice(*device);
            defaultIdx = i;
        }
    }

    // If the user has not manually changed device in list, we take default system audio device
    if(!hasManuallyChosenDevice)
        ui->comboBox->setCurrentIndex(defaultIdx);
    // Otherwise, we keep his choice
    else if(chosenDeviceIndex < ui->comboBox->count() && chosenDeviceIndex >= 0)
        ui->comboBox->setCurrentIndex(chosenDeviceIndex);

    musicPlayer->setSource(QUrl::fromLocalFile(sourceStr));
    musicPlayer->setLoops(QMediaPlayer::Infinite);
    if(musicPlayer->error() == QMediaPlayer::NoError)
    {
        if(!isOnPause)
        {
            musicPlayer->play();
            ui->pushButton->setIcon(QIcon(":/img/pause.png"));
        }
        else
        {
            ui->pushButton->setIcon(QIcon(":/img/resume.png"));
        }

        dontDisturbPlaying = true;
    }
}

void MusicPlayerWidget::stopMusic()
{
    chosenDeviceIndex = ui->comboBox->currentIndex();
    dontDisturbPlaying = false;
    if(musicPlayer->error() == QMediaPlayer::NoError)
        musicPlayer->stop();
}

void MusicPlayerWidget::setMusicTitle(QString str)
{
    ui->musicTitleLabel->setText(str);
}

void MusicPlayerWidget::scaleToScreen()
{
    QFont font = ui->label->font();
    font.setPointSize(font.pointSize() * property("heightFactor").toReal());
    ui->label->setFont(font);
    ui->comboBox->setFont(font);
    ui->musicTitleLabel->setFont(font);

    ui->positionSlider->setFixedHeight(ui->positionSlider->sizeHint().height()
                * property("heightFactor").toReal());
    ui->pushButton->setFixedHeight(42 * property("heightFactor").toReal());
    ui->pushButton->setIconSize(QSize (42 * property("widthFactor").toReal(), 42 * property("heightFactor").toReal()));
    ui->volumeSlider->setFixedHeight(ui->volumeSlider->sizeHint().height()
                * property("heightFactor").toReal());
}

void MusicPlayerWidget::onPositionChanged(qint64 milliseconds)
{
    if(musicPlayer->error() == QMediaPlayer::NoError)
    {
        if(musicPlayer->duration() != 0)
            ui->positionSlider->setValue((int)10000*milliseconds/musicPlayer->duration());
        else
            ui->positionSlider->setValue(0);
    }
}

void MusicPlayerWidget::onCurrentDeviceChanged(int index)
{
    auto devices = QMediaDevices::audioOutputs();
    if( index < devices.size() && index >= 0)
    {
        device = new QAudioDevice(devices.at(index));
        audioOutput->setDevice(*device);
        hasManuallyChosenDevice = true;
    }
}

void MusicPlayerWidget::onSliderPositionChanged(int val)
{
    if(musicPlayer->error() == QMediaPlayer::NoError)
    {
        QObject::disconnect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
        musicPlayer->setPosition((qint64)val*musicPlayer->duration()/10000);
        QObject::connect(musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
    }
}

void MusicPlayerWidget::onSliderVolumeChanged(int val)
{
    audioOutput->setVolume((float)val/100.);
}

void MusicPlayerWidget::onResumeOrPause()
{
    if(musicPlayer->error() == QMediaPlayer::NoError)
    {
        if(isOnPause)
        {
            musicPlayer->play();
            ui->pushButton->setIcon(QIcon(":/img/pause.png"));
            isOnPause = false;
        }
        else
        {
            musicPlayer->pause();
            ui->pushButton->setIcon(QIcon(":/img/resume.png"));
            isOnPause = true;
        }
    }
}
