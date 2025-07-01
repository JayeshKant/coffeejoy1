#pragma once
#include <QObject>

class Simulation : public QObject {
    Q_OBJECT
public:
    Simulation(QObject *p = nullptr) : QObject(p) {}
    bool pumpWaterCalled = false;
    int lastWaterNeeded = 0;
    int lastFlowRate = 0;
    void pumpWater(int water, int rate) {
        pumpWaterCalled = true;
        lastWaterNeeded = water;
        lastFlowRate = rate;
    }
signals:
    void waterDispensed();
};
