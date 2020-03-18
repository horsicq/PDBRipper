#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T09:14:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdbripper
TEMPLATE = app



SOURCES +=\
    dialogabout.cpp \
    dialogexport.cpp \
    dialogoptions.cpp \
    ../qwinpdb.cpp \
    dialogprocess.cpp \
    ../pdbprocess.cpp \
    guimainwindow.cpp \
    main_gui.cpp

HEADERS  += \
    ../global.h \
    ../qwinpdb.h \
    ../qwinpdb_def.h \
    dialogabout.h \
    dialogexport.h \
    dialogoptions.h \
    dialogprocess.h \
    ../pdbprocess.h \
    guimainwindow.h

FORMS    += \
        dialogabout.ui \
        dialogexport.ui \
        dialogoptions.ui \
        dialogprocess.ui \
        guimainwindow.ui

VC_PATH = "C:\Program Files (x86)\Microsoft Visual Studio 14.0"
LIBS += $$VC_PATH"\DIA SDK\lib\diaguids.lib"

INCLUDEPATH += $$VC_PATH"\DIA SDK\include"
DEPENDPATH += $$VC_PATH"\DIA SDK\include"


RC_ICONS = ../icons/main.ico

include(../build.pri)

RESOURCES += \
    resources.qrc
