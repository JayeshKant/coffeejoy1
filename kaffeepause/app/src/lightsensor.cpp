#include "lightsensor.h"
#include "simulation.h"
#include "coffeestatemachine.h"

//TODO when time left -> lightsensors in unterklassen trennen, die von lightsensors erben

LightSensor::LightSensor(Simulation* m_simulation, lightSensors sensor , QObject *parent) : QObject(parent), m_simulation(m_simulation), sensor(sensor) {

    timeOutLimit = new QTimer(this);
    connect(timeOutLimit, &QTimer::timeout, this, [this] (){
        emit timeoutError("Timeout: No Coin detected");
        this->pollDetection->stop();
        this->timeOutLimit->stop();
    });


    waitingForCup = new QTimer(this);
    connect(waitingForCup, &QTimer::timeout, this, &LightSensor::searchCup);


    pollLightSensor = new QTimer(this);
    connect(pollLightSensor, &QTimer::timeout, this, &LightSensor::getDetectionFromSimulation);


    pollDetection = new QTimer(this);
    connect(pollDetection, &QTimer::timeout, this, &LightSensor::pollDetectCoin);



}

LightSensor::~LightSensor() {}



void LightSensor::setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine){
    this->m_coffeeStateMachine = m_coffeeStateMachine;
}

void LightSensor::start(){
    if (sensor == lightSensors::cupInserted) {
        waitingForCup->start(1000);
    }
}

void LightSensor::reset() {
    this->pollLightSensor->stop();
    this->pollDetection->stop();
    this->timeOutLimit->stop();

    if (sensor == lightSensors::cupInserted) {
        waitingForCup->start(1000);
    }

    qDebug("LightSensor::reset");
    detectionCounter = 0;
    verifydetection = false;
    statusBegin = lightReceived;
    detectChangeInCup = false;
}

void LightSensor::shutdown() {
    this->pollLightSensor->stop();
    this->pollDetection->stop();
    this->timeOutLimit->stop();
    this->waitingForCup->stop();
}


void LightSensor::detectedCoinObject(detection statusBegin){

    qDebug() << "LightSensor::detectedCoinObject: ";
    this->detectionCounter = 0;
    qDebug() << "detectionCounter" << detectionCounter;
    this->verifydetection = false;
    qDebug() << "verifyDetection" << verifydetection;

    this->statusBegin = statusBegin;

    timeOutLimit->start(7000);

    emit waitingForDetectedCoin(sensor);

    pollLightSensor->start(100);
    qDebug() << "LightSensor::detectedCoinObject first detectLight()" << this->detectLight() << "StatusBegin: " << this->statusBegin;

    pollDetection->start(50);

}

void LightSensor::pollDetectCoin(){
    // qDebug() << "LightSensor::detectedCoinObject: pollDetection statusBegin "
    //          << statusBegin << "detectLight: " << this->detectLight();
    if(detectionCounter == 0 && statusBegin != this->detectLight()){
        detectionCounter = 1;
        qDebug() << "LightSensor::detectedCoinObject detection counter = 1";
    }else if(detectionCounter == 1 && statusBegin == this->detectLight()){
        verifydetection = true;
        emit coinDetectionSuccess(verifydetection); // TODO why do u need a bool here? Emit is already clear that it was a success
        qDebug() << "LightSensor::detectedCoinObject verifydetection = true";
        detectionCounter = 0;
        this->pollDetection->stop();
        this->timeOutLimit->stop();
    }else{
        // qDebug() << "LightSensor::detectedCoinObject next round poll Detection";
    }
}


int LightSensor::detectLevelInMicrometer(coinType type){
    return m_simulation->getCoinLevel(type);
}

detection LightSensor::detectLight(){
    getDetectionFromSimulation();
    return this->statusLightSensor;
}

void LightSensor::searchCup(){
    getDetectionFromSimulation();
    state currentState = m_coffeeStateMachine->getCurrentState();

    if (currentState == state::waitingForCup && statusLightSensor == detection::lightBlocked){
        // Cup is inserted
        qDebug() << "LightSensor::searchCup Cup Inserted Detected";
        emit cupInserted();
    } else if (currentState == state::takeOutCoffee && statusLightSensor == detection::lightReceived) {
        qDebug() << "LightSensor::searchCup Cup Taken";
        m_coffeeStateMachine->trigger(event::FINISHED);
        emit cupTaken();
    }
}

void LightSensor::getDetectionFromSimulation(){
    this->statusLightSensor = m_simulation->getDetection(this->sensor);
    //qDebug() << "LightSensor::getDetectionFromSimulation Sensor " << (int)this->sensor << "status " << (int)this->statusLightSensor;
}

enum lightSensors LightSensor::getSensorName(){
    return sensor;
}
