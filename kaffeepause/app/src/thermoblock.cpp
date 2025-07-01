#include "thermoblock.h"
#include "simulation.h"


Thermoblock::Thermoblock(Simulation* m_simulation, QObject* parent) : QObject(parent), m_simulation(m_simulation)  {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Thermoblock::controlHeating);
    timer->start(1000); // alle 1000 ms (1 Sekunde) //TODO start at start, stop at shutdown
}

Thermoblock::~Thermoblock() {}

void Thermoblock::start(){
    qDebug("Thermoblock::start");
    setCurrentMode(mode::standby);
}

void Thermoblock::reset(){
    qDebug("Thermoblock::reset");
    setCurrentMode(mode::standby);
}

void Thermoblock::setCurrentMode(mode newMode){
    this->currentMode = newMode;
    setTargetTemperature(currentMode);
}

mode Thermoblock::getCurrentMode(){
    return this->currentMode;
}

void Thermoblock::setTargetTemperature(mode newMode){
    switch (newMode) {
    case mode::brewing:
        this->targetTemperature = 90;
        break;
    case mode::steaming:
        this->targetTemperature = 120;
        break;
    case mode::maintenance:
        this->targetTemperature = 100;
        break;
    default:
        this->targetTemperature = 60;
        break;
    }
}

int Thermoblock::getTargetTemperature(){
    return this->targetTemperature;
}

bool Thermoblock::isAtTargetTemperature(){
    int currentTemperature = m_simulation->getCurrentTemperature();
    if (targetTemperature <= currentTemperature) {
        return true;
    } else {
        return false;
    }
}

void Thermoblock::controlHeating(){
    if (m_simulation->getCurrentWaterAmount() < 100){
        //TODO debug, water handling
        // qDebug() << "Not enough water";
        return; // if not enough Water in Thermoblock -> Thermoblock should not heat up TODO warning?
    }

    // qDebug() << "Thermoblock::controlHeating enough water";
    if (isAtTargetTemperature()){
        switch (currentMode) {
        case mode::brewing:
            qDebug() << "Thermoblock::controlHeating current Mode: brewing, Temperature Ok, setting Mode to Standby";
            setCurrentMode(mode::standby);
            emit waterTemperatureOK();
            break;
        case mode::steaming:
            qDebug() << "Thermoblock::controlHeating current Mode: steaming, Temperature Ok, setting Mode to Standby";
            setCurrentMode(mode::standby);
            emit steamingOK();
            break;
        case mode::standby:
           // qDebug() << "Thermoblock::controlHeating mode standby, Temperature Ok";
            // TODO is there something that should happen here?
            break;
        default:
            break;
        }
    } else {
        int newTemperature = m_simulation->getCurrentTemperature() + 5; // 5 degrees Celsius per second
        qDebug() << "Thermoblock::controlHeating increasing Temperatur, new Temperature: "  << newTemperature;
        m_simulation->setCurrentTemperature(newTemperature);
        emit temperatureChanged(newTemperature);   // 🔥

    }
}

