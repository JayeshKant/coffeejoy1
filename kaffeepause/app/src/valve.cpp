#include "valve.h"

Valve::Valve(valveType type, QObject* parent) : QObject(parent), type(type){

};

Valve::~Valve(){};

void Valve::reset(){
    qDebug("Valve::reset");
    currentValveState = valveState::closed;
}

void Valve::setValveState(valveState newState){
    qDebug () << "Valve::setValveState Type: " << (int)type << "New State: " << int(newState);
    this->currentValveState = newState;
    emit valveStateChanged(type, newState);
}

valveState Valve::getValveState(){
    return this->currentValveState;
}
