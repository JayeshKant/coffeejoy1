QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Using C++17 standard
CONFIG += c++17

# Your custom version info
VERSION = 0.1

# Directories for builds
DESTDIR = dist
OBJECTS_DIR = build
UI_DIR = gen/ui
MOC_DIR = gen/moc
RCC_DIR = gen/rcc

QMAKE_CXXFLAGS += -v

# disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Directories that are searched when including header files, no need for explicit directories, e.g. #include "../../code/foo.h" in source files
INCLUDEPATH += $$PWD/include

# The source files (e.g. *.cpp)
SOURCES += \
    src/internalview.cpp \
    src/splashscreen.cpp \
    src/coffeestatemachine.cpp \
    src/lightsensor.cpp \
    src/brewingunit.cpp \
    src/coffeeselection.cpp \
    src/coffeewaiter.cpp \
    src/coinchecker.cpp \
    src/coinsensor.cpp \
    src/coinsupply.cpp \
    src/grinder.cpp \
    src/main.cpp \
    src/maintenance.cpp \
    src/mainwindow.cpp \
    src/milkunit.cpp \
    src/payment.cpp \
    src/preparationshutdown.cpp \
    src/pump.cpp \
    src/pumpcontrol.cpp \
    src/simulation.cpp \
    src/startupmanager.cpp \
    src/thermoblock.cpp \
    src/touchhandler.cpp \
    src/touchscreen.cpp \
    src/valve.cpp

# The header files (e.g. *.h)
HEADERS += \
    include/brewingunit.h \
    include/buttonTypes.h \
    include/coffeeselection.h \
    include/coffeestatemachine.h \
    include/coffeetypes.h \
    include/coinchecker.h \
    include/grinder.h \
    include/mainWindow.h \
    include/coffeewaiter.h \
    include/maintenance.h \
    include/milkunit.h \
    include/payment.h \
    include/preparationshutdown.h \
    include/pump.h \
    include/pumpcontrol.h \
    include/startupmanager.h \
    include/thermoblock.h \
    include/touchhandler.h \
    include/touchscreen.h \
    include/valve.h \
    include/coinsensor.h \
    include/coinsupply.h \
    include/lightsensor.h \
    include/simulation.h \
    include/splashscreen.h \
    include/internalview.h

# The GUI definition files (so-called forms)
FORMS += \
    internalview.ui \
    mainwindow.ui \
    mainwindow2.ui \
    splashscreen.ui \

# Resources to integrate (e.g. images)
RESOURCES += \
    data/data.qrc

# Other files to include in the distribution
DISTFILES += \
#    ChatGPT Image Apr 21, 2025, 04_12_15 PM.png \
    data/Milk.png \
    data/Water.png \
    data/machine.png \
    data/B1.png \
    data/B2.png \
    data/CDis.png \
    data/Kaffee.png \
    data/MDis.png \
    data/MPump.png \
    data/WPump.png
    default.cfg

#----------------------------log

#* TODO logging
# Path to vcpkg
# VCPKG_ROOT = C:\Users\Jayes\vcpkg
# VCPKG_TARGET_TRIPLET = x64-windows

# INCLUDEPATH += $$VCPKG_ROOT/installed/$$VCPKG_TARGET_TRIPLET/src  #src oder include here?? TODO
# LIBS += -L$$VCPKG_ROOT/installed/$$VCPKG_TARGET_TRIPLET/lib -lspdlog
