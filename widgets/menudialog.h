#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>

namespace Ui {
class MenuDialog;
}

class MenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent = nullptr);
    ~MenuDialog();
signals:
    void changeToEnglishLanguage();
    void changeToFrenchLanguage();
    void EGopenLineColorSelection();
    void EGresetLevels();
    void openAbout();
    void openSourcesLink();
    void quitApp();
protected:
    void changeEvent(QEvent* event) override;
private:
    Ui::MenuDialog *ui;
};

#endif // MENUDIALOG_H
