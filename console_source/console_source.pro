QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = pdbripperc

include(../build.pri)

SOURCES += \
    ../pdbprocess.cpp \
    ../qwinpdb.cpp \
    main_console.cpp \
    consoleoutput.cpp

HEADERS += \
    ../pdbprocess.h \
    ../qwinpdb.h \
    ../qwinpdb_def.h \
    consoleoutput.h

INCLUDEPATH += ../msdia
SOURCES += ../msdia/diaCreate.cpp
