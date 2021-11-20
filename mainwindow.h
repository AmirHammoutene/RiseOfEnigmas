#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStackedWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "EulerGraphInteract/eulergraphinteract.h"
#include "EulerGraphInteract/vertex.h"
#include "EulerGraphInteract/edge.h"

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
    QMenuBar *menubar;
    QStackedWidget *stackedWidget;
    QScrollArea *HPmainWidget; // HP for "Home Page"
    QGridLayout *HPmainLayout;
    QPushButton *HPEulerGraphButton;
    QLabel *HPEulerGraphLabel;
    QPushButton *HPcomingSoonButton;
    QLabel *HPcomingSoonLabel;
    QVBoxLayout *HPEulerGraphLayout;
    QVBoxLayout *HPcomingSoonLayout;
    EulerGraphInteract *eulerGraph; // Eulerian Graphs module
    QScrollArea *EGmainWidget; // EG for "Euler Graphs"
    QGridLayout *EGmainLayout;
    QHBoxLayout *EGtextAndButtonLayout;
    QCheckBox *EGeasyMode;
    QPushButton *EGhomeButton;
    QLabel *EGstateLabel;
    QHash< uint, QPair< QList<Vertex> , QList<Edge> > > EulerGraphStageData; // Eulerian Graphs vertexes and edges are hard stored
    uint EGscore;
    bool EGclickbyClickMode;
    QColor EGlineColor;
    int EGcustomColorPosition;
    QAction * THselectDefaultStyleAction; // TH for "Theme"
    QAction * THselectDarkStyleAction;
    QString THstyleName;

private:
    Ui::MainWindow *ui;
    void finishCreate(); // Create central widget content
    void constructData(); // Create vertexes and edges for Eulerian Graphs

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onAbout();
    void onReadme();
    void onDownloadLink();
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

signals:
    void EGlineColorRequest(QColor color);
};
#endif // MAINWINDOW_H
