#include "menudialog.h"
#include "ui_menudialog.h"

MenuDialog::MenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QDialog {background-color: rgba( 60, 60, 60, 0% );}");
    QObject::connect(ui->englishButton, SIGNAL(released()), this, SIGNAL(changeToEnglishLanguage()));
    QObject::connect(ui->frenchButton, SIGNAL(released()), this, SIGNAL(changeToFrenchLanguage()));
    QObject::connect(ui->EGlineColorButton, SIGNAL(released()), this, SIGNAL(EGopenLineColorSelection()));
    QObject::connect(ui->EGresetButton, SIGNAL(released()), this, SIGNAL(EGresetLevels()));
    QObject::connect(ui->aboutButton, SIGNAL(released()), this, SIGNAL(openAbout()));
    QObject::connect(ui->sourcesButton, SIGNAL(released()), this, SIGNAL(openSourcesLink()));
    QObject::connect(ui->quitButton, SIGNAL(released()), this, SIGNAL(quitApp()));
}

MenuDialog::~MenuDialog()
{
    delete ui;
}

void MenuDialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
       ui->retranslateUi(this);
    }
    QDialog::changeEvent(event);
}
