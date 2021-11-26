#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QCheckBox>
#include "EulerGraphInteract/eulergraphinteract.h"
#include "EulerGraphInteract/vertex.h"
#include "EulerGraphInteract/edge.h"
#include "widgets/eulergraphstimechallengewidget.h"
#include "widgets/musicplayerwidget.h"

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
    QAction * THselectDefaultStyleAction; // TH for "Theme"
    QAction * THselectDarkStyleAction;
    QString THstyleName;
    MusicPlayerWidget *musicPlayerWidget;

private:
    Ui::MainWindow *ui;
    void finishCreate(); // Create central widget content
    void constructData(); // Create vertexes and edges for Eulerian Graphs

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void onAbout();
    void onReadme();
    void onDownloadLink1();
    void onDownloadLink2();
    void onMusicLink();
    void onSourcesLink();
    void goToHomehPage();
    void goToEulerGraphPage();
    void applyDefaultStyle();
    void applyDarkStyle();
    void applyStyleSheet(QString ssName);
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
