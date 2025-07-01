QT += widgets testlib
QT -= gui


CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# Using C++17 standard
CONFIG += c++17

# Directories that are searched when including header files, no need for explicit directories, e.g. #include "../../code/foo.h" in source files
INCLUDEPATH += $$PWD/../app/include

SOURCES += \
    ../app/src/pump.cpp \
    unittest_pump.cpp

HEADERS += \
    ../app/include/pump.h \
    unittest_pump.h

TARGET = unittest_pump
