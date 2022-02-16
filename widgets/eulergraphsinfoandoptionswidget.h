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
    void scaleToScreen();

public slots:
    void setMagnetModeChecked(bool val);
    void setInstructionsText(const QString & str);
signals:
    void homePageRequest();
    void resetEulerGraphScoreRequest();
    void chooseEGlineColorRequest();
    void magnetModeStateChanged(int);
    void translateInstructionsRequest();
protected:
    void changeEvent(QEvent* event) override;
private:
    Ui::EulerGraphsInfoAndOptionsWidget *ui;
};

#endif // EULERGRAPHSINFOANDOPTIONSWIDGET_H
