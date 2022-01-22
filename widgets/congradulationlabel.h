#ifndef CONGRADULATIONLABEL_H
#define CONGRADULATIONLABEL_H

#include <QObject>
#include <QLabel>
#include <QMovie>

class CongradulationLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CongradulationLabel(QWidget *parent = nullptr);
    ~CongradulationLabel();
private:
    QMovie *gif;
signals:

};

#endif // CONGRADULATIONLABEL_H
