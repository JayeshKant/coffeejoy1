#include "milkunit.h"

MilkUnit::MilkUnit(
    Thermoblock* m_thermoblock,
    CoffeeStateMachine* m_coffeeStateMachine,
    Simulation* m_simulation,
    BrewingUnit* m_brewingUnit,
    CoffeeWaiter* m_coffeeWaiter,
    PumpControl* m_pumpControl,
    Pump* m_milkPump,
    Valve* m_steamValve,
    Valve* m_milkValve, QObject* parent)
    : QObject(parent),
    m_coffeeStateMachine(m_coffeeStateMachine),
    m_simulation(m_simulation),
    m_pumpControl(m_pumpControl),
    m_brewingUnit(m_brewingUnit),
    m_thermoblock(m_thermoblock),
    m_milkPump(m_milkPump),
    m_milkValve(m_milkValve),
    m_steamValve(m_steamValve),
    m_coffeeWaiter(m_coffeeWaiter){

    qDebug() << "Initialize MilkUnit";

    connect(m_pumpControl, &PumpControl::targetPressureReached, this, &MilkUnit::onPressureReached);
    connect(m_brewingUnit, &BrewingUnit::brewingFinished, this, &MilkUnit::createSteam);
    connect(m_simulation, &Simulation::milkDispensed, this, &MilkUnit::onMilkDispensed);
    connect(m_thermoblock, &Thermoblock::steamingOK, this, &MilkUnit::steamMilk);

    qDebug() << "Simulation: Initialize and Start Timer for MilkCooling";
    updateMilkCoolingTimer = new QTimer(this);
    connect(updateMilkCoolingTimer, &QTimer::timeout, this, &MilkUnit::reduceMilkTemperature); // TODO stop when shutting down start with startup

    qDebug() << "Simulation: Initialize and Start Timer for pollMilkTemperature";
    pollMilkTemperature = new QTimer(this);
    connect(pollMilkTemperature, &QTimer::timeout, this, &MilkUnit::coolMilk);
    pollMilkTemperature->start(5000);

    //TODO end with shtudown and start with start
}

MilkUnit::~MilkUnit() {}

void MilkUnit::reduceMilkTemperature(){
    int milkTemperature = m_simulation->getMilkTemperature();
    m_simulation->setMilkTemperature(milkTemperature-1);
}

void MilkUnit::reset(){
    qDebug("MilkUnit::reset");
    this->milkNeeded = 0;
    this->steamingMilk = false;
}

void MilkUnit::createSteam(){
    m_coffeeWaiter->heatUpWater(mode::steaming);
}

void MilkUnit::steamMilk(){
    qDebug() << "MilkUnit::steamMilk starting pump";
    steamingMilk = true;
    m_pumpControl->setMilkDispense(true);
}

void MilkUnit::onPressureReached(){
    if (!steamingMilk) return;

    qDebug() << "MilkUnit::onPressureReached";

    milkNeeded = m_coffeeWaiter->getAmountMilk();

    qDebug() << "MilkUnit::onPressureReached Amount of Water: " << milkNeeded;

    m_steamValve->setValveState(valveState::open);
    m_milkValve->setValveState(valveState::open);

    m_simulation->pumpMilk(milkNeeded, flowRateMilk, flowRateWater);
}


void MilkUnit::onMilkDispensed(){
    m_steamValve->setValveState(valveState::closed); // Ventil schließen
    m_milkValve->setValveState(valveState::closed); // Ventil schließen

    m_pumpControl->setPressureReached(false);
    m_pumpControl->setMilkDispense(false);
    steamingMilk = false;

    m_coffeeStateMachine->trigger(event::COFFEE_READY_TO_TAKEOUT);
    emit coffeeReady();

}

int MilkUnit::getMilkToWarm(){
    return milkToWarm;
}

void MilkUnit::coolMilk(){
    if(m_simulation->getMilkTemperature() > 8){
        milkToWarm += 1;
        if(!updateMilkCoolingTimer->isActive()){
            qDebug() << "MilkUnit::updateMilkCoolingTimer start Temperature: " << m_simulation->getMilkTemperature();
            updateMilkCoolingTimer->start(5000);
        }

    }else if(m_simulation->getMilkTemperature() <= 3){
        qDebug() << "MilkUnit::updateMilkCoolingTimer start Temperature: " << m_simulation->getMilkTemperature();
        updateMilkCoolingTimer->stop();
        if (milkToWarm < 30){
            milkToWarm = 0;
        }
    }
}


