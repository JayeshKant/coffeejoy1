#pragma once

#include "touchscreen.h"
#include "coinsensor.h"
#include "pump.h"
#include "valve.h"
#include "simulation.h"
#include "payment.h"
#include "coffeeselection.h"
#include "coffeestatemachine.h"
#include "lightsensor.h"
#include "thermoblock.h"
#include "grinder.h"
#include "coinchecker.h"
#include "coinsupply.h"
#include "pumpcontrol.h"
#include "brewingunit.h"
#include "milkunit.h"
#include "maintenance.h"
#include "touchhandler.h"
#include "milkunit.h"

class DummyTouchScreen : public TouchScreen {
public:
    DummyTouchScreen(QObject* parent = nullptr) : TouchScreen(parent) {}
};

class DummyCoinSensor : public CoinSensor {
public:
    DummyCoinSensor(QObject* parent = nullptr) : CoinSensor(coinType::EUR1, parent) {}
};

class DummyPump : public Pump {
public:
    DummyPump(QObject* parent = nullptr) : Pump(parent) {}
};

class DummyValve : public Valve {
public:
    DummyValve(valveType type = valveType::water, QObject* parent = nullptr)
        : Valve(type, parent) {}
};

class DummySimulation : public Simulation {
public:
    DummySimulation(QObject* parent = nullptr)
        : Simulation(new DummyTouchScreen(parent), new DummyCoinSensor(parent),
                     new DummyPump(parent), new DummyValve(valveType::supply, parent), parent) {}
};

class DummyPayment : public Payment {
public:
    DummyPayment(QObject* parent = nullptr)
        : Payment(new DummySimulation(parent), new CoffeeSelection(parent), parent) {}
};

class DummyCoffeeSelection : public CoffeeSelection {
public:
    DummyCoffeeSelection(QObject* parent = nullptr) : CoffeeSelection(parent) {}

    int getPriceInCent() override {
        return 150;  // Beispielpreis
    }
};


class DummyLightSensor : public LightSensor {
public:
    DummyLightSensor(QObject* parent = nullptr)
        : LightSensor(new DummySimulation(parent), lightSensors::cupInserted, parent) {

    }
};

class DummyCoinSupply : public CoinSupply {
public:
    DummyCoinSupply(QObject* parent = nullptr)
        : CoinSupply(new DummySimulation(parent),
                     createDummyCoinSensors(parent),
                     createDummyLightSensors(parent),
                     parent) {}

private:
    static std::array<CoinSensor*, 6> createDummyCoinSensors(QObject* parent) {
        return {
            new DummyCoinSensor(parent), new DummyCoinSensor(parent),
            new DummyCoinSensor(parent), new DummyCoinSensor(parent),
            new DummyCoinSensor(parent), new DummyCoinSensor(parent)
        };
    }

    static std::array<LightSensor*, 2> createDummyLightSensors(QObject* parent) {
        return {
            new DummyLightSensor(parent), new DummyLightSensor(parent)
        };
    }
};

class DummyCoinChecker : public CoinChecker {
public:
    DummyCoinChecker(QObject* parent = nullptr)
        : CoinChecker(new DummyCoinSensor(parent), parent) {}
};


class DummyCoffeeStateMachine : public CoffeeStateMachine {
public:
    DummyCoffeeStateMachine(QObject* parent = nullptr)
        : CoffeeStateMachine(
              new DummySimulation(parent),
              new DummyPayment(parent),
              new DummyCoinChecker(parent),
              new DummyCoffeeSelection(parent),
              new DummyCoinSupply(parent),
              parent) {}
};



class DummyThermoblock : public Thermoblock {
public:
    DummyThermoblock(QObject* parent = nullptr) : Thermoblock(new DummySimulation(parent), parent) {}
};

class DummyGrinder : public Grinder {
public:
    DummyGrinder(QObject* parent = nullptr)
        : Grinder(new DummyCoffeeStateMachine(parent), nullptr, new DummySimulation(parent), parent) {}
};





class DummyPumpControl : public PumpControl {
public:
    DummyPumpControl(QObject* parent = nullptr)
        : PumpControl(new DummySimulation(parent), new DummyPump(parent), parent) {}
};

class DummyBrewingUnit : public BrewingUnit {
public:
    DummyBrewingUnit(QObject* parent = nullptr)
        : BrewingUnit(new DummyCoffeeStateMachine(parent),
                      new DummySimulation(parent),
                      nullptr,
                      new DummyPumpControl(parent),
                      new DummyValve(valveType::water, parent),
                      parent) {}
};

class DummyMilkUnit : public MilkUnit {
public:
    DummyMilkUnit(QObject* parent = nullptr)
        : MilkUnit(new DummyThermoblock(parent),
                   new DummyCoffeeStateMachine(parent),
                   new DummySimulation(parent),
                   new DummyBrewingUnit(parent),
                   nullptr,
                   new DummyPumpControl(parent),
                   new DummyPump(parent),
                   new DummyValve(valveType::steam, parent),
                   new DummyValve(valveType::milk, parent),
                   parent) {}
};

class DummyMaintenance : public Maintenance {
public:
    DummyMaintenance(QObject* parent = nullptr)
        : Maintenance(new DummyCoffeeStateMachine(parent),
                      new DummySimulation(parent),
                      new DummyPumpControl(parent),
                      new DummyPump(parent),
                      new DummyValve(valveType::water, parent),
                      new DummyThermoblock(parent),
                      new DummyValve(valveType::milk, parent),
                      array<LightSensor*, 7>{},
                      new DummyValve(valveType::supply, parent),
                      new DummyMilkUnit(parent),
                      parent) {}
};

class DummyTouchHandler : public TouchHandler {
public:
    DummyTouchHandler(QObject* parent = nullptr)
        : TouchHandler(new DummyCoffeeStateMachine(parent), new DummyTouchScreen(parent), parent) {}
};
