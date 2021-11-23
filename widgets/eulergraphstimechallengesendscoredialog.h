#ifndef EULERGRAPHSTIMECHALLENGESENDSCOREDIALOG_H
#define EULERGRAPHSTIMECHALLENGESENDSCOREDIALOG_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class EulerGraphsTimeChallengeSendScoreDialog;
}

class EulerGraphsTimeChallengeSendScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EulerGraphsTimeChallengeSendScoreDialog(QWidget *parent = nullptr);
    ~EulerGraphsTimeChallengeSendScoreDialog();
    void loadOrGeneratePseudo();
    void setTimeScore(const QString & timeScore);
    QString timeScoreStr;
    float timeScoreFl;
    bool inError;
    QString tempStr;
    void showErrorDialog();
public slots :
    void sendScoreOnline();
    void sendScoreRequestFinished(QNetworkReply *reply);
private:
    Ui::EulerGraphsTimeChallengeSendScoreDialog *ui;
};

#endif // EULERGRAPHSTIMECHALLENGESENDSCOREDIALOG_H
