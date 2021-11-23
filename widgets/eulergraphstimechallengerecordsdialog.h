#ifndef EULERGRAPHSTIMECHALLENGERECORDSDIALOG_H
#define EULERGRAPHSTIMECHALLENGERECORDSDIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QHeaderView>

namespace Ui {
class EulerGraphsTimeChallengeRecordsDialog;
}

class EulerGraphsTimeChallengeRecordsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EulerGraphsTimeChallengeRecordsDialog(QWidget *parent = nullptr);
    ~EulerGraphsTimeChallengeRecordsDialog();
    bool inError;
    QString errorString;
    void loadRecords();
    void showErrorDialog();
public slots:
    void showAndLoad();
    void sendScoreRequestFinished(QNetworkReply *reply);

private:
    Ui::EulerGraphsTimeChallengeRecordsDialog *ui;
    QHeaderView *header;
};

#endif // EULERGRAPHSTIMECHALLENGERECORDSDIALOG_H
