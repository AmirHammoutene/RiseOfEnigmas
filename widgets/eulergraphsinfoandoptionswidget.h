#ifndef EULERGRAPHSINFOANDOPTIONSWIDGET_H
#define EULERGRAPHSINFOANDOPTIONSWIDGET_H

#include <QWidget>

namespace Ui {
class EulerGraphsInfoAndOptionsWidget;
}

class EulerGraphsInfoAndOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EulerGraphsInfoAndOptionsWidget(QWidget *parent = nullptr);
    ~EulerGraphsInfoAndOptionsWidget();

public slots:
    void setClickByClickModeChecked(bool val);
    void setInstructionsText(const QString & str);
signals:
    void homePageRequest();
    void resetEulerGraphScoreRequest();
    void chooseEGlineColorRequest();
    void easyModeStateChanged(int);
private:
    Ui::EulerGraphsInfoAndOptionsWidget *ui;
};

#endif // EULERGRAPHSINFOANDOPTIONSWIDGET_H
