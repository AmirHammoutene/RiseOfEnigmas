#ifndef EULERGRAPHSTIMECHALLENGEWIDGET_H
#define EULERGRAPHSTIMECHALLENGEWIDGET_H

#include "eulergraphstimechallengesendscoredialog.h"
#include "eulergraphstimechallengerecordsdialog.h"

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
    EulerGraphsTimeChallengeRecordsDialog *recordsDialog;
    void scaleToScreen();
public slots:
    void updateTime();
    void startOrStopChrono();
    void showSendScoreDialog();
    void goToChallengePage();
signals:
    void startChallengeRequest();
protected:
    void changeEvent(QEvent* event) override;
private:
    Ui::EulerGraphsTimeChallengeWidget *ui;
};

#endif // EULERGRAPHSTIMECHALLENGEWIDGET_H
