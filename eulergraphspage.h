#ifndef EULERGRAPHSPAGE_H
#define EULERGRAPHSPAGE_H

#include <QScrollArea>
#include <QPushButton>

#include "EulerGraphInteract/eulergraphinteract.h"
#include "EulerGraphInteract/vertex.h"
#include "EulerGraphInteract/edge.h"
#include "widgets/eulergraphstimechallengewidget.h"
#include "widgets/eulergraphsinfoandoptionswidget.h"
#include "widgets/congradulationdialog.h"

class MainWindow;
class EulerGraphGenerationPage;

class EulerGraphsPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit EulerGraphsPage(QWidget *parent = nullptr); //must have a MainWindow as a parent
    EulerGraphInteract *eulerGraph; // Eulerian Graphs module
    EulerGraphsTimeChallengeWidget *EGtimeChallengeWidget;// EG for "Euler Graphs"
    EulerGraphsInfoAndOptionsWidget *EGinfoOptionsWidget;
    QMap< uint, QPair< QList<Vertex> , QList<Edge> > > EulerGraphStageData; // Eulerian Graphs vertexes and edges are hard stored
    uint EGscore;
    uint currentGraphNum;
    bool EGclickbyClickMode;
    QColor EGlineColor;
    int EGcustomColorPosition;
    CongradulationDialog *congradulationPopup;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *generationPageButton;

    void scaleToScreen();
    bool m_isValid = false;
    void resetPage();
    void refreshShow();
    void checkButtonsAvailability();

    void inheritEulerGraphGenerationPageSettings(EulerGraphGenerationPage *eulerGraphGenerationPage);

private:
    void constructData(); // Create vertexes and edges for Eulerian Graphs
    void finishCreate();
    MainWindow *m_parent;

protected:
    void changeEvent(QEvent* event) override;

public slots:
    void chooseEGlineColor();
    void resetEulerGraphScore();
    void changeEGMagnetMode(int status);
    void EGStepedUp(uint step, uint total);
    void setNextEulerGraph();
    void EGstartChallenge();
    void translateEGInstructions();
    void goToPreviousGraph();
    void goToNextGraph();
    void goToGraphGenerationPage();

signals:
    void EGlineColorRequest(QColor color);

};

#endif // EULERGRAPHSPAGE_H
