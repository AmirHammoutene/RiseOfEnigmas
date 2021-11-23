#include "eulergraphstimechallengesendscoredialog.h"
#include "ui_eulergraphstimechallengesendscoredialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

inline QString const GameJolt_private_key = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";// In the context of open-source code, this is private info

EulerGraphsTimeChallengeSendScoreDialog::EulerGraphsTimeChallengeSendScoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EulerGraphsTimeChallengeSendScoreDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Send time score online"));
    ui->errorLabel->setStyleSheet("QLabel {color:red;border: 1px solid;}");
    ui->errorLabel->setVisible(false);
    ui->pseudoLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[A-Za-z0-9_\\- ]*")));
    loadOrGeneratePseudo();
    timeScoreFl = 999.9;
    timeScoreStr = "999.9";
    inError = false;

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendScoreOnline()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(QDialog::reject()));
}

EulerGraphsTimeChallengeSendScoreDialog::~EulerGraphsTimeChallengeSendScoreDialog()
{
    delete ui;
}

void EulerGraphsTimeChallengeSendScoreDialog::loadOrGeneratePseudo()
{
    QSettings settings;
    QString loaded = settings.value("EGpseudo", "ProGamer").toString();

    if(loaded == "ProGamer")
    {
        // We generate a pseudonym with numbers
        int randomInt = rand() % 9000 + 1000;
        ui->pseudoLineEdit->setText(loaded+QString::number(randomInt));
    }
    else
        ui->pseudoLineEdit->setText(loaded);
}

void EulerGraphsTimeChallengeSendScoreDialog::setTimeScore(const QString &timeScore)
{
    ui->timeScoreLabel->setText(timeScore+tr(" seconds"));
    bool ok;
    timeScoreStr = timeScore,
    timeScoreFl = timeScore.toFloat(&ok);
    if(ok)
    {
        inError = false;
        ui->errorLabel->setVisible(false);
    }
    else
    {
        inError = true;
        ui->errorLabel->setText(tr("Internal error: conversion to float"));
        ui->errorLabel->setVisible(true);
    }
}

void EulerGraphsTimeChallengeSendScoreDialog::sendScoreOnline()
{
    QSettings settings;
    settings.setValue("EGpseudo", ui->pseudoLineEdit->text());

    if(ui->pseudoLineEdit->text().isEmpty())
    {
        inError = true;
        ui->errorLabel->setText(tr("User edit error: Pseudonym entered is empty"));
        ui->errorLabel->setVisible(true);
    }
    if(inError)
    {
        showErrorDialog();
        return;
    }

    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

    QString game_id = "665470";
    QString guest = ui->pseudoLineEdit->text();
    QString score = timeScoreStr;
    QString sort = QString::number((int)timeScoreFl*10);
    QString table_id = "674033";

    QUrl urlQueryPart("https://api.gamejolt.com/api/game/v1_2/scores/add/"
        "?game_id="+game_id+"&guest="+guest+"&score="+score+"&sort="+sort+"&table_id="+table_id);

    if(!urlQueryPart.isValid())
    {
        inError = true;
        ui->errorLabel->setText(tr("HTTP URL parsing error: The pseudonym entered seems to have an unexpected character.\n")+urlQueryPart.errorString());
        ui->errorLabel->setVisible(true);
        showErrorDialog();
        return;
    }

    QString strToHash = urlQueryPart.toString()+GameJolt_private_key;
    // Generate signature of query including private key to be able to send the query (Game Jolt security process)
    QString signature = QString::fromUtf8(QCryptographicHash::hash(strToHash.toUtf8(), QCryptographicHash::Md5).toHex());

    QNetworkRequest request(QUrl(urlQueryPart.toString()+"&signature="+signature));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    tempStr = request.url().toString();
    networkManager->post(request,QByteArray());

    QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(sendScoreRequestFinished(QNetworkReply*)));

    QDialog::accept();
}

void EulerGraphsTimeChallengeSendScoreDialog::sendScoreRequestFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        inError = true;
        ui->errorLabel->setText(tr("Network Reply error: ")+QString(reply->errorString()));
        ui->errorLabel->setVisible(true);
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
            ui->errorLabel->setText(tr("Response parsing error: ")+QString(jsonParseError.errorString()));
            ui->errorLabel->setVisible(true);
            showErrorDialog();
            return;
        }
        QJsonObject jsonObj = json.object();
        QJsonValue jsonVal = jsonObj.value("response");
        QJsonObject jsonItemObj = jsonVal.toObject();
        QJsonValue jsonSubVal = jsonItemObj.value("success");
        if(jsonSubVal.toString() != "true")
        {
            inError = true;
            jsonSubVal = jsonItemObj.value("message");
            ui->errorLabel->setText(tr("Storing via GameJolt API error: ")+jsonSubVal.toString()+"\n"+
                                    tempStr);
            ui->errorLabel->setVisible(true);
            showErrorDialog();
            return;
        }

        QMessageBox::information(this, tr("Score sent"),tr("Your score has been successfully sent."));
    }
}



void EulerGraphsTimeChallengeSendScoreDialog::showErrorDialog()
{
    QMessageBox::critical(this, tr("Unable to send score"),tr("An error occured, your score has not been integrated.\n\nError message:\n")+ui->errorLabel->text());
}

