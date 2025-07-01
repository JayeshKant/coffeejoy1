#pragma once

#include <QObject>

#include "pumpcontrol.h"
#include "valve.h"
#include "coffeewaiter.h"
#include "simulation.h"
#include "coffeestatemachine.h"


class BrewingUnit : public QObject {

    Q_OBJECT

public:

    BrewingUnit(
        CoffeeStateMachine* m_coffeeStateMachine,
        Simulation* m_simulation,
        CoffeeWaiter* m_coffeeWaiter,
        PumpControl* m_pumpControl,
        Valve* m_waterValve,
        QObject* parent = nullptr);
    ~BrewingUnit();

    void reset();

    void moveWaste();
    void brewCoffee();



signals:

    void brewingFinished();
    void coffeeReady();

private slots:
    void onPressureReached();
    void onWaterDispensed();


private:
    PumpControl* m_pumpControl;
    Simulation* m_simulation;
    CoffeeStateMachine* m_coffeeStateMachine;

    Valve* m_waterValve;
    Valve* m_steamValve;

    CoffeeWaiter* m_coffeeWaiter;

    int flowRate = 5; // ml / s

    int waterNeeded = 0;

    bool brewingCoffee = false;

};

