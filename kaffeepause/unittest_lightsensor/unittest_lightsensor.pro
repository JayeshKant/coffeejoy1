QT += widgets testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

# Using C++17 standard
CONFIG += c++17

# Directories that are searched when including header files
INCLUDEPATH += $$PWD/../app/include

SOURCES += \
    ../app/src/lightsensor.cpp \
    ../app/src/simulation.cpp \
    ../app/src/coffeestatemachine.cpp \
    ../app/src/coffeeselection.cpp \
    ../app/src/payment.cpp \
    ../app/src/coinchecker.cpp \
    ../app/src/coinsupply.cpp \
    ../app/src/coinsensor.cpp \
    ../app/src/touchscreen.cpp \
    ../app/src/touchhandler.cpp \
    ../app/src/pump.cpp \
    ../app/src/pumpcontrol.cpp \
    ../app/src/valve.cpp \
    unittest_lightsensor.cpp

HEADERS += \
    ../app/include/lightsensor.h \
    ../app/include/simulation.h \
    ../app/include/coffeestatemachine.h \
    ../app/include/coffeeselection.h \
    ../app/include/payment.h \
    ../app/include/coinchecker.h \
    ../app/include/coinsupply.h \
    ../app/include/coinsensor.h \
    ../app/include/touchscreen.h \
    ../app/include/touchhandler.h \
    ../app/include/pump.h \
    ../app/include/pumpcontrol.h \
    ../app/include/valve.h \
    ../app/include/buttonTypes.h \
    ../app/include/coffeetypes.h \
    unittest_lightsensor.h

TARGET = unittest_lightsensor
