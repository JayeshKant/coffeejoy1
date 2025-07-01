#pragma once

#include <QObject>

#include "pumpcontrol.h"
#include "valve.h"
#include "coffeewaiter.h"
#include "brewingunit.h"
#include "thermoblock.h"

class MilkUnit : public QObject {

    Q_OBJECT

public:

    MilkUnit(
        Thermoblock* m_thermoblock,
        CoffeeStateMachine* m_coffeeStateMachine,
        Simulation* m_simulation,
        BrewingUnit* m_brewingUnit,
        CoffeeWaiter* m_coffeeWaiter,
        PumpControl* m_pumpControl,
        Pump* m_milkPump,
        Valve* m_steamValve,
        Valve* m_milkValve,
        QObject* parent = nullptr);
    ~MilkUnit();

    void reset();
    void coolMilk();
    int getMilkToWarm();

signals:
    void coffeeReady();

private slots:
    void createSteam();
    void steamMilk();
    void onPressureReached();
    void onMilkDispensed();

private:

    CoffeeStateMachine* m_coffeeStateMachine;
    Simulation* m_simulation;
    PumpControl* m_pumpControl;
    BrewingUnit* m_brewingUnit;
    Thermoblock* m_thermoblock;
    Pump* m_milkPump;

    int milkToWarm = 0;

    Valve* m_milkValve;
    Valve* m_steamValve;

    QTimer* updateMilkCoolingTimer;
    QTimer* pollMilkTemperature;

    void reduceMilkTemperature();

    CoffeeWaiter* m_coffeeWaiter;

    const int flowRateMilk = 10;
    const int flowRateWater = 5;
    int milkNeeded = 0;
    bool steamingMilk = false;
};

