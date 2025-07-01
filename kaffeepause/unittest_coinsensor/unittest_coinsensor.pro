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
    ../app/src/coffeeselection.cpp \
    ../app/src/coinsensor.cpp \
    ../app/src/payment.cpp \
    ../app/src/simulation.cpp \
    ../app/src/touchhandler.cpp \
    ../app/src/pumpcontrol.cpp \
    ../app/src/coinchecker.cpp \
    ../app/src/coffeestatemachine.cpp \
    ../app/src/coinsupply.cpp \
    ../app/src/lightsensor.cpp \
    ../app/src/coffeewaiter.cpp \
    ../app/src/grinder.cpp \
    ../app/src/thermoblock.cpp \
    ../app/src/pump.cpp \
    ../app/src/touchscreen.cpp \
    ../app/src/valve.cpp \
    unittest_coinsensor.cpp

HEADERS += \
    ../app/include/coffeetypes.h \
    ../app/include/coffeeselection.h \
    ../app/include/coinsensor.h \
    ../app/include/payment.h \
    ../app/include/coindata.h \
    ../app/include/simulation.h \
    ../app/include/touchhandler.h \
    ../app/include/pumpcontrol.h \
    ../app/include/coinchecker.h \
    ../app/include/coffeestatemachine.h \
    ../app/include/coinsupply.h \
    ../app/include/lightsensor.h \
    ../app/include/coffeewaiter.h \
    ../app/include/grinder.h \
    ../app/include/thermoblock.h \
    ../app/include/pump.h \
    ../app/include/touchscreen.h \
    ../app/include/buttonTypes.h \
    ../app/include/valve.h \
    unittest_coinsensor.h

TARGET = unittest_coinsensor
