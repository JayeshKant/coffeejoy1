#include "brewingunit.h"


BrewingUnit::BrewingUnit(CoffeeStateMachine* m_coffeeStateMachine, Simulation* m_simulation, CoffeeWaiter* m_coffeeWaiter,PumpControl* m_pumpControl, Valve* m_waterValve, QObject* parent)
    : QObject(parent), m_pumpControl(m_pumpControl), m_simulation(m_simulation), m_coffeeStateMachine(m_coffeeStateMachine), m_waterValve(m_waterValve), m_coffeeWaiter(m_coffeeWaiter) {

    qDebug() << "Initialize BrewingUnit";
    connect(m_pumpControl, &PumpControl::targetPressureReached, this, &BrewingUnit::onPressureReached);
    connect(m_coffeeWaiter, &CoffeeWaiter::startBrewing, this, &BrewingUnit::brewCoffee);
    connect(m_simulation, &Simulation::waterDispensed, this, &BrewingUnit::onWaterDispensed);
}

BrewingUnit::~BrewingUnit() {}

void BrewingUnit::reset() {
    qDebug("BrewingUnit::reset");
    flowRate = 5;
    waterNeeded = 0;
    brewingCoffee = false;
}


void BrewingUnit::moveWaste() {
    // TODO move Waste somewhere (Simulation add Waste part)
}

void BrewingUnit::brewCoffee() {
    qDebug() << "BrewingUnit::brewCoffee starting pump";
    brewingCoffee = true;
    m_pumpControl->setTargetPressure(pressure);
    m_pumpControl->setBrewCoffee(true);
}

void BrewingUnit::onPressureReached(){
    qDebug() << "BrewingUnit::onPressureReached";
    if (!brewingCoffee) return;

    waterNeeded = m_coffeeWaiter->getAmountWater();
    qDebug() << "BrewingUnit::onPressureReached Amount of Water: " << waterNeeded;

    m_waterValve->setValveState(valveState::open);

    m_simulation->pumpWater(waterNeeded, flowRate);
}


void BrewingUnit::onWaterDispensed(){
    if (brewingCoffee){
        m_waterValve->setValveState(valveState::closed); // Ventil schließen
        brewingCoffee = false;
        moveWaste();
        if (m_coffeeWaiter->getTypeCoffee() == coffee::cappuccino) {
            m_coffeeStateMachine->trigger(event::BREWING_COMPLETET);
            m_pumpControl->setBrewCoffee(false);
            m_pumpControl->setPressureReached(false);
            emit brewingFinished();
        } else {
            m_coffeeStateMachine->trigger(event::COFFEE_READY_TO_TAKEOUT);
            emit coffeeReady();
        }
    }
}


