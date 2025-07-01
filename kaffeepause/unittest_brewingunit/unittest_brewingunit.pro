QT += core testlib
QT -= gui
CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/stubs
INCLUDEPATH += $$PWD/../app/include


SOURCES += \
    ../app/src/brewingunit.cpp \
     tst_brewingunit.cpp

HEADERS += \

    ../app/include/brewingunit.h \
    stubs/coffeestatemachine.h \
    stubs/pumpcontrol.h \
    stubs/simulation.h \
    stubs/coffeewaiter.h \
    stubs/coffeestatemachine.h \
    stubs/valve.h \
    stubs/coffeeinformation.h


