#include "eulergraphstimechallengerecordsdialog.h"
#include "ui_eulergraphstimechallengerecordsdialog.h"
#include "PrivateData.h"

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>

EulerGraphsTimeChallengeRecordsDialog::EulerGraphsTimeChallengeRecordsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EulerGraphsTimeChallengeRecordsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Time records of Eulerian Graphs Challenge"));

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    inError = false;
}

EulerGraphsTimeChallengeRecordsDialog::~EulerGraphsTimeChallengeRecordsDialog()
{
    delete ui;
}

void EulerGraphsTimeChallengeRecordsDialog::loadRecords()
{
    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QString game_id = "665470";
    QString limit = "100";
    QString table_id = "674033";

    QUrl urlQueryPart("http://api.gamejolt.com/api/game/v1_2/scores/"
        "?game_id="+game_id+"&table_id="+table_id+"&limit="+limit);

    if(!urlQueryPart.isValid())
    {
        inError = true;
        errorString = tr("HTTP URL parsing error: ")+urlQueryPart.errorString();
        showErrorDialog();
        return;
    }

    QString strToHash = urlQueryPart.toString()+PrivateData::GameJolt_private_key;
    // Generate signature of query including private key to be able to send the query (Game Jolt security process)
    QString signature = QString::fromUtf8(QCryptographicHash::hash(strToHash.toUtf8(), QCryptographicHash::Md5).toHex());

    QNetworkRequest request(QUrl(urlQueryPart.toString()+"&signature="+signature));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkManager->post(request,QByteArray());

    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendScoreRequestFinished(QNetworkReply*)));
    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply *)), networkManager, SLOT(deleteLater()));
}

void EulerGraphsTimeChallengeRecordsDialog::showErrorDialog()
{
    QMessageBox::critical(this, tr("Unable fetch scores table"),tr("An error occured, records can't be retrieved.\n\nError message:\n")
                          +errorString);
}

void EulerGraphsTimeChallengeRecordsDialog::showAndLoad()
{
    show();
    loadRecords();
}

void EulerGraphsTimeChallengeRecordsDialog::sendScoreRequestFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        inError = true;
        errorString = tr("Network Reply error: ")+QString(reply->errorString());
        showErrorDialog();
        return;
    }
    else
    {
        QByteArray response = reply->readAll();
        QJsonParseError jsonParseError;
        QJsonDocument json = QJsonDocument::fromJson(response,&jsonParseError);
        if(jsonParseError.error != QJsonParseError::NoError)
        {
            inError = true;
            errorString = tr("Response parsing error: ")+QString(jsonParseError.errorString());
            showErrorDialog();
            return;
        }
        QJsonObject jsonObj = json.object();
        QJsonValue jsonVal = jsonObj.value("response");
        QJsonObject jsonItemObj = jsonVal.toObject();
        QJsonValue jsonSuccessVal = jsonItemObj.value("success");
        if(jsonSuccessVal.toString() != "true")
        {
            inError = true;
            jsonSuccessVal = jsonItemObj.value("message");
            errorString = tr("Storing via GameJolt API error: ")+jsonSuccessVal.toString();
            showErrorDialog();
            return;
        }

        QJsonArray jsonScoresArray = jsonItemObj.value("scores").toArray();

        QStandardItemModel *model = new QStandardItemModel(ui->tableView);

        for (auto v : jsonScoresArray) {
            QJsonObject aScoreElement = v.toObject();
            QString aName = aScoreElement.value("guest").toString();
            QString aScore = aScoreElement.value("score").toString();
            model->appendRow(QList<QStandardItem*>() << new QStandardItem(aName) << new QStandardItem(aScore));
        }

        model->setHorizontalHeaderLabels(QStringList() << tr("Pseudonym") << tr("Score"));
        ui->tableView->setModel(model);
    }
}
