QT += core gui
greaterThan(QT_MAJOR_VERSION,4): QT += widgets
greaterThan(QT_MAJOR_VERSION,5): QT += core5compat

CONFIG += c++1z

TEMPLATE = app

CONFIG(release, debug|release):TARGET = RiseOfEnigmas
CONFIG(debug, debug|release):TARGET = RiseOfEnigmasd

DEPENDSPATH += $$PWD/EulerGraphInteract
INCLUDEPATH += $$PWD/EulerGraphInteract
include($$PWD/EulerGraphInteract/EulerGraphInteract.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

RC_FILE = resource.rc

TRANSLATIONS += \
    RiseOfEnigmas_fr.ts
