#include "congradulationdialog.h"
#include "ui_congradulationdialog.h"
#include "widgets/congradulationlabel.h"

CongradulationDialog::CongradulationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CongradulationDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QDialog {background-color: rgba( 60, 60, 60, 0% );}");
    CongradulationLabel *congradulationLabel = new CongradulationLabel(this);
    ui->horizontalLayout->addWidget(congradulationLabel,0, Qt::AlignHCenter);
    close();
}

CongradulationDialog::~CongradulationDialog()
{
    delete ui;
}

void CongradulationDialog::mousePressEvent(QMouseEvent *event)
{
    close();

    QDialog::mousePressEvent(event);
}
