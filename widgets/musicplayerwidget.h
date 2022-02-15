#ifndef MUSICPLAYERWIDGET_H
#define MUSICPLAYERWIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioDevice>

namespace Ui {
class MusicPlayerWidget;
}

class MusicPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayerWidget(QWidget *parent = nullptr);
    ~MusicPlayerWidget();
    QMediaPlayer *musicPlayer;
    QAudioOutput *audioOutput;
    QAudioDevice *device;
    bool dontDisturbPlaying;
    static bool hasManuallyChosenDevice;
    static int chosenDeviceIndex;
    void setMusicAndPlayLoop(QString sourceStr);
    void stopMusic();
    static const int DefaultVolume;
    void setMusicTitle(QString str);
    bool isOnPause;
    void scaleToScreen();
public slots:
    void onPositionChanged(qint64 milliseconds);
    void onCurrentDeviceChanged(int index);
    void onSliderPositionChanged(int val);
    void onSliderVolumeChanged(int val);
    void onResumeOrPause();
private:
    Ui::MusicPlayerWidget *ui;
};

#endif // MUSICPLAYERWIDGET_H
