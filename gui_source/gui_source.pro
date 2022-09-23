QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pdbripper
TEMPLATE = app

SOURCES +=\
    ../qwinpdb.cpp \
    ../pdbprocess.cpp \
    dialogabout.cpp \
    dialogexport.cpp \
    dialogoptions.cpp \
    dialogprocess.cpp \
    guimainwindow.cpp \
    main_gui.cpp

HEADERS += \
    ../global.h \
    ../qwinpdb.h \
    ../qwinpdb_def.h \
    ../pdbprocess.h \
    dialogabout.h \
    dialogexport.h \
    dialogoptions.h \
    dialogprocess.h \
    guimainwindow.h

FORMS += \
    dialogabout.ui \
    dialogexport.ui \
    dialogoptions.ui \
    dialogprocess.ui \
    guimainwindow.ui

INCLUDEPATH += ../msdia
SOURCES += ../msdia/diaCreate.cpp

RC_ICONS = ../icons/main.ico

include(../build.pri)

!contains(XCONFIG, xoptionswidget) {
    XCONFIG += xoptionswidget
    include(../XOptions/xoptionswidget.pri)
}

!contains(XCONFIG, xwinpdb) {
    XCONFIG += xwinpdb
    include(../XWinPDB/xwinpdb.pri)
}

!contains(XCONFIG, xaboutwidget) {
    XCONFIG += xaboutwidget
    include(../XAboutWidget/xaboutwidget.pri)
}

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../changelog.txt \
    ../release_version.txt
