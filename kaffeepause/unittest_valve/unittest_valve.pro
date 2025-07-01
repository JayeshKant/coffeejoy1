QT += core testlib
QT -= gui
CONFIG += qt console testcase
CONFIG -= app_bundle
INCLUDEPATH += $$PWD/../app/include

SOURCES += \
    ../app/src/valve.cpp \
    tst_valve.cpp
HEADERS += \
    ../app/include/valve.h \
