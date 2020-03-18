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

VC_PATH = "C:\Program Files (x86)\Microsoft Visual Studio 14.0"
LIBS += $$VC_PATH"\DIA SDK\lib\diaguids.lib"

INCLUDEPATH += $$VC_PATH"\DIA SDK\include"
DEPENDPATH += $$VC_PATH"\DIA SDK\include"
