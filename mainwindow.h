#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QCheckBox>
#include <QTranslator>
#include "EulerGraphInteract/eulergraphinteract.h"
#include "EulerGraphInteract/vertex.h"
#include "EulerGraphInteract/edge.h"
#include "widgets/eulergraphstimechallengewidget.h"
#include "widgets/musicplayerwidget.h"
#include "widgets/menudialog.h"
#include "widgets/congradulationdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Vertex;
class Edge;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStackedWidget *stackedWidget;
    EulerGraphInteract *eulerGraph; // Eulerian Graphs module
    QLabel *EGstateLabel;// EG for "Euler Graphs"
    QCheckBox *EGeasyMode;
    EulerGraphsTimeChallengeWidget *EGtimeChallengeWidget;
    QHash< uint, QPair< QList<Vertex> , QList<Edge> > > EulerGraphStageData; // Eulerian Graphs vertexes and edges are hard stored
    uint EGscore;
    bool EGclickbyClickMode;
    QColor EGlineColor;
    int EGcustomColorPosition;
    MusicPlayerWidget *musicPlayerWidget;
    MenuDialog *menuDialog;
    QTranslator *translator;
    QTranslator *baseTranslator;

    QLabel *HPEulerGraphLabel;
    QLabel *HPcomingSoonLabel;
    QPushButton  *EGchangeLineColorButton;
    QPushButton  *EGresetButton;
    QPushButton *EGhomeButton;

    CongradulationDialog *congradulationPopup;

private:
    Ui::MainWindow *ui;
    void finishCreate(); // Create central widget content
    void constructData(); // Create vertexes and edges for Eulerian Graphs

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void changeEvent(QEvent* event) override;

public slots:
    void changeToEnglishLanguage();
    void changeToFrenchLanguage();
    void onAbout();
    void onSourcesLink();
    void goToHomehPage();
    void goToEulerGraphPage();
    void applyStyleSheet();
    void chooseEGlineColor();
    void resetEulerGraphScore();
    void changeEGClickMode(int status);
    void EGStepedUp(uint step, uint total);
    void setNextEulerGraph();
    void EGstartChallenge();

signals:
    void EGlineColorRequest(QColor color);
};
#endif // MAINWINDOW_H
