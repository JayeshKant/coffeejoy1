#pragma once

#include <QObject>
#include <QEventLoop>

#include "brewingunit.h"
#include "coffeeselection.h"
#include "coffeestatemachine.h"
#include "coffeewaiter.h"
#include "coinchecker.h"
#include "grinder.h"
#include "lightsensor.h"
#include "maintenance.h"
#include "milkunit.h"
#include "payment.h"
#include "pump.h"
#include "pumpcontrol.h"
#include "simulation.h"
#include "thermoblock.h"
#include "touchhandler.h"
#include "touchscreen.h"
#include "valve.h"

using std::array;

class StartUpManager : public QObject
{
    Q_OBJECT
public:
    StartUpManager(
        BrewingUnit* m_brewingUnit,
        CoffeeSelection* m_coffeeSelection,
        CoffeeStateMachine* m_coffeeStateMachine,
        CoffeeWaiter* m_coffeeWaiter,
        CoinChecker* m_coinChecker,
        Grinder* m_grinder,
        array<LightSensor*, 9> m_lightSensors,
        Maintenance* m_maintenance,
        MilkUnit* m_milkUnit,
        Payment* m_payment,
        Pump* m_pump,
        PumpControl* m_pumpControl,
        Simulation* m_simulation,
        Thermoblock* m_thermoblock,
        TouchHandler* m_touchHandler,
        TouchScreen* m_touchScreen,
        array<Valve*, 4> m_valves,
        QObject *parent = nullptr);
    ~StartUpManager();

    void startUp();
    void resetAll();
    void onMaintenance();

    bool getResetDone() { return this->resetDone; };

signals:
    void startUpComplete();
    void resetComplete();
    void attentionNeeded();


private:
    BrewingUnit* m_brewingUnit;
    CoffeeSelection* m_coffeeSelection;
    CoffeeStateMachine* m_coffeeStateMachine;
    CoffeeWaiter* m_coffeeWaiter;
    CoinChecker* m_coinChecker;
    Grinder* m_grinder;
    array<LightSensor*, 9> m_lightSensors;
    Maintenance* m_maintenance;
    MilkUnit* m_milkUnit;
    Payment* m_payment;
    Pump* m_pump;
    PumpControl* m_pumpControl;
    Simulation* m_simulation;
    Thermoblock* m_thermoblock;
    TouchHandler* m_touchHandler;
    TouchScreen* m_touchScreen;
    Valve* m_waterValve;
    Valve* m_steamValve;
    Valve* m_milkValve;
    Valve* m_freshWaterSupplyValve;

    QTimer* waitingForResetComplete = new QTimer(this);


    bool resetDone = false;

};
