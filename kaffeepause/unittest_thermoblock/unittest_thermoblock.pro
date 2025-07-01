QT += testlib
QT -= gui
CONFIG += qt console testcase c++17
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../app/include

SOURCES += \
    ../app/src/thermoblock.cpp \
    ../app/src/simulation.cpp \
    ../app/src/pump.cpp \
    ../app/src/valve.cpp \
    ../app/src/touchscreen.cpp \
    ../app/src/coinsensor.cpp \
    ../app/src/coinsupply.cpp \
    ../app/src/coffeeselection.cpp \
    ../app/src/coffeestatemachine.cpp \
    ../app/src/coinchecker.cpp \
    ../app/src/grinder.cpp \
    ../app/src/payment.cpp \
    ../app/src/pumpcontrol.cpp \
    ../app/src/coffeewaiter.cpp \
    ../app/src/lightsensor.cpp \
    ../app/src/touchhandler.cpp \
    tst_thermoblock.cpp

HEADERS += \
    ../app/include/thermoblock.h \
    ../app/include/simulation.h \
    ../app/include/pump.h \
    ../app/include/valve.h \
    ../app/include/touchscreen.h \
    ../app/include/coinsensor.h \
    ../app/include/coinsupply.h \
    ../app/include/coffeeselection.h \
    ../app/include/coffeestatemachine.h \
    ../app/include/coinchecker.h \
    ../app/include/grinder.h \
    ../app/include/payment.h \
    ../app/include/pumpcontrol.h \
    ../app/include/coffeewaiter.h \
    ../app/include/touchhandler.h \
    ../app/include/lightsensor.h #\
    #stubs/simulation.h
