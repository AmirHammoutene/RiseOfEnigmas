#ifndef EULERGRAPHGENERATIONPAGE_H
#define EULERGRAPHGENERATIONPAGE_H

#include <QScrollArea>

#include "EulerGraphInteract/eulergraphinteract.h"
#include "widgets/eulergraphsinfoandoptionswidget.h"
#include "widgets/congradulationdialog.h"
#include "widgets/graphgenerationprogressdialog.h"

class MainWindow;
class EulerGraphsPage;

class EulerGraphGenerationPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit EulerGraphGenerationPage(QWidget *parent = nullptr); //must have a MainWindow as a parent
    EulerGraphInteract *eulerGraph; // Eulerian Graphs module
    EulerGraphsInfoAndOptionsWidget *EGinfoOptionsWidget;
    bool EGclickbyClickMode;
    QColor EGlineColor;
    int EGcustomColorPosition;
    CongradulationDialog *congradulationPopup;
    QPushButton *normalPageButton;
    QPushButton *generateButton;
    GraphGenerationProgressDialog *progressDialog;

    static const int minimumVerticesNumber;
    static const int maximumVerticesNumber;

    void scaleToScreen();
    bool m_isValid = false;
    void resetPage();
    void inheritEulerGraphsPageSettings(EulerGraphsPage *eulerGraphsPage);

private:
    void finishCreate();
    MainWindow *m_parent;
    EulerGraphsPage *m_eulerGraphsPage;

protected:
    void changeEvent(QEvent* event) override;

public slots:
    void chooseEGlineColor();
    void changeEGMagnetMode(int status);
    void EGStepedUp(uint step, uint total);
    void translateEGInstructions();
    void graphFinished();
    void goToNormalPage();
    void getGenerationEdgesNumber();
    void generatedGraphDataReceived(QPair< QList<Vertex> , QList<Edge> > data);
    void generationCancelled();
signals:
    void EGlineColorRequest(QColor color);
    void cancelGeneration();

};

#endif // EULERGRAPHGENERATIONPAGE_H
