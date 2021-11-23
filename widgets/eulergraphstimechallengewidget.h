#ifndef EULERGRAPHSTIMECHALLENGEWIDGET_H
#define EULERGRAPHSTIMECHALLENGEWIDGET_H

#include "eulergraphstimechallengesendscoredialog.h"

#include <QFrame>
#include <QElapsedTimer>
#include <QTimer>

namespace Ui {
class EulerGraphsTimeChallengeWidget;
}

class EulerGraphsTimeChallengeWidget : public QFrame
{
    Q_OBJECT

public:
    explicit EulerGraphsTimeChallengeWidget(QWidget *parent = nullptr);
    ~EulerGraphsTimeChallengeWidget();
    QElapsedTimer elapsedTimer;
    QTimer timer;
    bool running;
    void finishChallenge();
    EulerGraphsTimeChallengeSendScoreDialog *sendScoreDialog;
public slots:
    void updateTime();
    void startOrStopChrono();
    void showSendScoreDialog();
signals:
    void startChallengeRequest();
private:
    Ui::EulerGraphsTimeChallengeWidget *ui;
};

#endif // EULERGRAPHSTIMECHALLENGEWIDGET_H
