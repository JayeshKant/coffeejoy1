#include "pump.h"

Pump::Pump(QObject* parent) : QObject(parent) {}

Pump::~Pump() {}

void Pump::reset(){
    qDebug("Pump::reset");
    this->setRunning(false);
}

void Pump::setRunning(bool running){
    this->running = running;
}

bool Pump::isRunning(){
    return this->running;
}


