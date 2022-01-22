#include "congradulationlabel.h"

CongradulationLabel::CongradulationLabel(QWidget *parent)
    : QLabel{parent}
{
    gif = new QMovie(":/img/fireworks.gif");
    gif->start();
    setAttribute(Qt::WA_NoSystemBackground);
    setMovie(gif);
}

CongradulationLabel::~CongradulationLabel()
{
    gif->deleteLater();
}
