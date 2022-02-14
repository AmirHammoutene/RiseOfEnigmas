QT += core gui network
greaterThan(QT_MAJOR_VERSION,4): QT += widgets
greaterThan(QT_MAJOR_VERSION,5): QT += core5compat multimedia

CONFIG += c++1z

TEMPLATE = app

CONFIG(release, debug|release):TARGET = RiseOfEnigmas
CONFIG(debug, debug|release):TARGET = RiseOfEnigmasd

DEPENDSPATH += $$PWD/EulerGraphInteract
INCLUDEPATH += $$PWD/EulerGraphInteract
include($$PWD/EulerGraphInteract/EulerGraphInteract.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    widgets/congradulationdialog.cpp \
    widgets/congradulationlabel.cpp \
    widgets/eulergraphsinfoandoptionswidget.cpp \
    widgets/eulergraphstimechallengerecordsdialog.cpp \
    widgets/eulergraphstimechallengesendscoredialog.cpp \
    widgets/eulergraphstimechallengewidget.cpp \
    widgets/menudialog.cpp \
    widgets/musicplayerwidget.cpp

HEADERS += \
    PrivateData.h \
    mainwindow.h \
    widgets/congradulationdialog.h \
    widgets/congradulationlabel.h \
    widgets/eulergraphsinfoandoptionswidget.h \
    widgets/eulergraphstimechallengerecordsdialog.h \
    widgets/eulergraphstimechallengesendscoredialog.h \
    widgets/eulergraphstimechallengewidget.h \
    widgets/menudialog.h \
    widgets/musicplayerwidget.h

FORMS += \
    mainwindow.ui \
    widgets/congradulationdialog.ui \
    widgets/eulergraphsinfoandoptionswidget.ui \
    widgets/eulergraphstimechallengerecordsdialog.ui \
    widgets/eulergraphstimechallengesendscoredialog.ui \
    widgets/eulergraphstimechallengewidget.ui \
    widgets/menudialog.ui \
    widgets/musicplayerwidget.ui

RESOURCES += \
    resources.qrc

RC_FILE = resource.rc

TRANSLATIONS += \
    RiseOfEnigmas_fr.ts\
    RiseOfEnigmas_en.ts
