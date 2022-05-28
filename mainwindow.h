#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QTranslator>
#include "widgets/menudialog.h"
#include "eulergraphspage.h"
#include "eulergraphgenerationpage.h"
#include "widgets/musicplayerwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStackedWidget *stackedWidget;
    QPushButton *HPEulerGraphButton;// HP for "Home Page"
    QPushButton *HPcomingSoonButton;
    MenuDialog *menuDialog;
    QTranslator *translator;
    QTranslator *baseTranslator;

    QLabel *HPEulerGraphLabel;
    QLabel *HPcomingSoonLabel;

    EulerGraphsPage *EGmainWidget;
    EulerGraphGenerationPage *EGgenerationWidget;

    MusicPlayerWidget *musicPlayerWidget;

    void scaleToScreen();

private:
    Ui::MainWindow *ui;
    void finishCreate(); // Create central widget content

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

signals:
    void EGlineColorRequest(QColor color);
};
#endif // MAINWINDOW_H
