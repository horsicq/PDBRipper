QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = pdbripperc

include(../build.pri)

SOURCES += \
    ../pdbprocess.cpp \
    ../qwinpdb.cpp \
    ../qstaticpdb.cpp \
    main_console.cpp \
    consoleoutput.cpp

HEADERS += \
    ../pdbprocess.h \
    ../qwinpdb.h \
    ../qwinpdb_def.h \
    ../qstaticpdb.h \
    consoleoutput.h

INCLUDEPATH += ../msdia
SOURCES += ../msdia/diaCreate.cpp

!contains(XCONFIG, xpdb) {
    XCONFIG += xpdb
    include(../../_mylibs/XPDB/xpdb.pri)
}
