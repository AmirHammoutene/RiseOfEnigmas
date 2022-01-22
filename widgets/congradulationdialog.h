#ifndef CONGRADULATIONDIALOG_H
#define CONGRADULATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CongradulationDialog;
}

class CongradulationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CongradulationDialog(QWidget *parent = nullptr);
    ~CongradulationDialog();
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
private:
    Ui::CongradulationDialog *ui;
};

#endif // CONGRADULATIONDIALOG_H
