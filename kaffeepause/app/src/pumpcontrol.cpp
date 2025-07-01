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

    if (brewingCoffee || milkDispense) {
        startPump();
        if ((m_simulation->getCurrentPressure() >= targetPressure && !pressureReached)) {
            //qDebug() << "PumpControl::checkCurrentPressure target Pressure reached, stopping pump";
            qDebug() << "PumpControl::checkCurrentPressure currentPressure: " << m_simulation->getCurrentPressure();
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
