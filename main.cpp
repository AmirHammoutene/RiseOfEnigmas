/**
 * Conceived and developed by Amir Hammoutene (amir.hammoutene@gmail.com) in March 2021.
 * Under GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 *
 * This program is a series of puzzle games.
 *
 * The code uses a lot of Qt framework version 6. You'll find a bunch of documentation about Qt classes on internet and principally on https://doc.qt.io/
**/

#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    QApplication::setOrganizationName("Amir Hammoutene");
    a.setWindowIcon(QIcon(":/img/icon.png"));

    // Adjust widgets size to screen resolution
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    qreal refHeight = 1080.;
    qreal refWidth = 1920.;
    QRect screenRect = a.primaryScreen()->geometry();
    qreal heightFactor = screenRect.height() / refHeight;
    qreal widthFactor = screenRect.width() / refWidth;

    // Setup translations. It's based on the system local language, if custom translation files exists (.qm)
    QString locale = QLocale::system().name().section('_', 0, 0); // get system language
    QTranslator translator;
    if(translator.load(QString("RiseOfEnigmas_") + locale))
        a.installTranslator(&translator);// install app custom translator

    QTranslator baseTranslator;
    if(baseTranslator.load(QString("qtbase_") + locale))
        a.installTranslator(&baseTranslator);// install qt based translator (for boutons like "Cancel")

    MainWindow w;
    w.setProperty("heightFactor",heightFactor);
    w.setProperty("widthFactor",widthFactor);
    w.scaleToScreen();
    w.show();

    return a.exec();
}
