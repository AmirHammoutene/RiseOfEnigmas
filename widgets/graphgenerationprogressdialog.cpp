#include "graphgenerationprogressdialog.h"
#include "ui_graphgenerationprogressdialog.h"

GraphGenerationProgressDialog::GraphGenerationProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphGenerationProgressDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QDialog {background-color: qlineargradient(spread:repeat, x1:1, y1:0, x2:1, y2:1, stop:0 #7d7d7d,stop:1 #616161);}");
}

GraphGenerationProgressDialog::~GraphGenerationProgressDialog()
{
    delete ui;
}

void GraphGenerationProgressDialog::setStepTextAndProgressValue(uint step, QVariant otherInfo1, QVariant otherInfo2, QVariant otherInfo3, QVariant otherInfo4)
{
    ui->buttonBox->setVisible(false);
    QString stepText;
    int percentage = 0;

    switch (step) {
    case 0:
        stepText = tr("Initializing...");
        break;
    case 1:
        stepText = tr("Step 1 of 4 : Placing vertices randomly (")+QString::number(otherInfo1.toInt())+"/"+QString::number(otherInfo2.toInt())+")...";
        percentage = otherInfo2.toInt() != 0 ? 100*otherInfo1.toInt()/otherInfo2.toInt() : 0;
        break;
    case 2:
        stepText = tr("Step 2 of 4 : Creating edges randomly for each vertex (")+QString::number(otherInfo1.toInt())+"/"+QString::number(otherInfo2.toInt())+")...";
        percentage = otherInfo2.toInt() != 0 ? 100*otherInfo1.toInt()/otherInfo2.toInt() : 0;
        break;
    case 3:
        stepText = tr("Step 3 of 4 : Deleting one-degree vertices until we have no more than 2 (")+QString::number(otherInfo1.toInt())+tr(" left)...");
        percentage = otherInfo2.toInt()-2 > 0 ? 100*(otherInfo2.toInt()-otherInfo1.toInt())/(otherInfo2.toInt()-2) : 0;
        break;
    case 4:
        stepText = tr("Step 4 of 4 : Arranging to have full of even-degree vertices (step : ")+QString::number(otherInfo1.toInt())+tr(" vertex : ")+QString::number(otherInfo2.toInt())+"/"+QString::number(otherInfo3.toInt())+")...";
        percentage = otherInfo3.toInt() != 0 ? 100*otherInfo2.toInt()/otherInfo3.toInt() : 0;
        break;
    case 11:
        stepText = tr("Step 1 of 4 : Placing vertices randomly (")+QString::number(otherInfo1.toInt())+"/"+QString::number(otherInfo2.toInt())+")\n\n"+
                tr("It seems to take too much time. The process will terminate in few seconds...");
        percentage = otherInfo4.toInt() != 0 ? 100*(otherInfo4.toInt()-otherInfo3.toInt())/otherInfo4.toInt() : 0;
        break;
    case 14:
        stepText = tr("Step 4 of 4 : Arranging to have full of even-degree vertices (adding)\n\n")+
                tr("It seems to take too much time. The process will terminate in few seconds...");
        percentage = otherInfo2.toInt() != 0 ? 100*(otherInfo2.toInt()-otherInfo1.toInt())/otherInfo2.toInt() : 0;
        break;
    case 15:
        stepText = tr("Step 4 of 4 : Arranging to have full of even-degree vertices (deleting)\n\n")+
                tr("It seems to take too much time. The process will terminate in few seconds...");
        percentage = otherInfo2.toInt() != 0 ? 100*(otherInfo2.toInt()-otherInfo1.toInt())/otherInfo2.toInt() : 0;
        break;
    default:
        stepText = tr("Error while generating Eulerian Graph, please try again (error : ")+QString::number(step)+").";
        ui->buttonBox->setVisible(true);
        break;
    }
    ui->stepLabel->setText(stepText);
    ui->progressBar->setValue(percentage);
}

void GraphGenerationProgressDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
       ui->retranslateUi(this);
    }
    QDialog::changeEvent(event);
}
