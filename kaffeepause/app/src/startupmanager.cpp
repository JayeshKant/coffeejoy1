#include "startupmanager.h"

StartUpManager::StartUpManager(
    BrewingUnit* m_brewingUnit,
    CoffeeSelection* m_coffeeSelection,
    CoffeeStateMachine* m_coffeeStateMachine,
    CoffeeWaiter* m_coffeeWaiter,
    CoinChecker* m_coinChecker,
    Grinder* m_grinder,
    array<LightSensor*, 9> m_lightSensors,
    MilkUnit* m_milkUnit,
    Payment* m_payment,
    Pump* m_pump,
    PumpControl* m_pumpControl,
    Simulation* m_simulation,
    Thermoblock* m_thermoblock,
    TouchHandler* m_touchHandler,
    TouchScreen* m_touchScreen,
    array<Valve*, 4> m_valves,
    QObject *parent)
        : QObject(parent),
        m_brewingUnit(m_brewingUnit),
        m_coffeeSelection(m_coffeeSelection),
        m_coffeeStateMachine(m_coffeeStateMachine),
        m_coffeeWaiter(m_coffeeWaiter),
        m_coinChecker(m_coinChecker),
        m_grinder(m_grinder),
        m_lightSensors(m_lightSensors),
        m_milkUnit(m_milkUnit),
        m_payment(m_payment),
        m_pump(m_pump),
        m_pumpControl(m_pumpControl),
        m_simulation(m_simulation),
        m_thermoblock(m_thermoblock),
        m_touchHandler(m_touchHandler),
        m_touchScreen(m_touchScreen),
        m_waterValve(m_valves[0]),
        m_steamValve(m_valves[1]),
        m_milkValve(m_valves[2]),
        m_freshWaterSupplyValve(m_valves[3]){

    for (LightSensor* lightSensor : m_lightSensors) {
        if (lightSensor->getSensorName() == cupInserted) {
            connect(lightSensor, &LightSensor::cupTaken, this, &StartUpManager::resetAll);
        }

    }

    connect(m_touchHandler, &TouchHandler::startUp, this, &StartUpManager::startUp);

}


StartUpManager::~StartUpManager(){

}

void StartUpManager::startUp(){
    m_simulation->start();
    m_coinChecker->start();
    for (LightSensor* lightSensor : m_lightSensors) {
        lightSensor->start();
    }
    m_pumpControl->start();
    m_thermoblock->start();

}

void StartUpManager::resetAll(){ //reset everything, that should be reset by new start
    m_brewingUnit->reset();
    m_coffeeWaiter->reset();
    m_coffeeSelection->reset();
    m_grinder->reset();
    m_milkUnit->reset();
    m_payment->reset();
    m_pump->reset();
    m_pumpControl->reset();
    m_simulation->reset();
    m_thermoblock->reset();
    m_touchHandler->reset();

    for (LightSensor* lightSensor : m_lightSensors) {
        lightSensor->reset();
    }

    emit resetComplete();
}
