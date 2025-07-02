#include "pumpcontrol.h"

PumpControl::PumpControl(Simulation* m_simulation, Pump* m_pump, QObject* parent)
    : QObject(parent), m_pump(m_pump), m_simulation(m_simulation) {

    pressureTimer = new QTimer(this);
    pressureTimer->setInterval(1000);
    connect(pressureTimer, &QTimer::timeout, this, &PumpControl::checkCurrentPressure);

}

PumpControl::~PumpControl(){}

void PumpControl::start(){
    pressureTimer->start();
    brewingCoffee = false;
    milkDispense = false;
    pressureReached = false;
}

void PumpControl::reset(){
    if (!pressureTimer->isActive()){
        pressureTimer->start();
    }
    brewingCoffee = false;
    milkDispense = false;
    pressureReached = false;
    flushSystem = false;
}


void PumpControl::startPump(){
    if (!m_pump->isRunning()){
        m_pump->setRunning(true);
        qDebug() << "PumpControl::startPump set Pump to true";
    }
}

void PumpControl::stopPump(){
    if (m_pump->isRunning()){
        m_pump->setRunning(false);
    }
}

void PumpControl::checkCurrentPressure(){
    // start and stop pump, dependend on currentPressure

    if (brewingCoffee || milkDispense || flushSystem) {
        startPump();
        qDebug() << "PumpControl::checkCurrentPressure currentPressure: " << m_simulation->getCurrentPressure();
        if ((m_simulation->getCurrentPressure() >= targetPressure && !pressureReached)) {
            pressureReached = true;
            emit targetPressureReached();
        }
    } else {
        stopPump();
    }
}

int PumpControl::getTargetPressure(){
    return this->targetPressure;
}

void PumpControl::setTargetPressure(int targetPressure){
    this->targetPressure = targetPressure;
}

void PumpControl::setBrewCoffee(bool brewingCoffee){
    this->brewingCoffee = brewingCoffee;
}

void PumpControl::setMilkDispense(bool milkDispense){
    this->milkDispense = milkDispense;
}

void PumpControl::setPressureReached(bool pressureReached){
    this->pressureReached = pressureReached;
}

void PumpControl::setFlushSystem(bool flushSystem){
    this->flushSystem = flushSystem;
}
