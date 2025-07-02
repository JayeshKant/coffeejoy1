#include "maintenance.h"


Maintenance::Maintenance(CoffeeStateMachine* m_coffeeStateMachine,
                         Simulation* m_simulation, PumpControl* m_pumpControl,
                         Pump* m_waterPump, Valve* m_waterValve,
                         Thermoblock* m_thermoblock, Valve* m_milkValve,
                         std::array<LightSensor*,7> m_maintenanceLightSensors,
                         Valve* m_freshWaterSupplyValve, MilkUnit* m_milkUnit, QObject* parent):
                        QObject(parent),
                        m_pumpControl(m_pumpControl),
                         m_simulation(m_simulation), m_coffeeStateMachine(m_coffeeStateMachine),
                         m_waterValve(m_waterValve), m_milkValve(m_milkValve),
                         m_freshWaterSupplyValve(m_freshWaterSupplyValve),
                        m_waterPump(m_waterPump), m_thermoblock(m_thermoblock),
                        m_milkUnit(m_milkUnit)
{

    cremaBeanLevelLightSensor = m_maintenanceLightSensors.at(0);
    espressoBeanLevelLightSensor = m_maintenanceLightSensors.at(1);
    milkLevelLightSensor = m_maintenanceLightSensors.at(2);
    dirtyWaterLightSensor = m_maintenanceLightSensors.at(3);
    wasteDisposaleLightSensor = m_maintenanceLightSensors.at(4);
    cupInsertedLightSensor = m_maintenanceLightSensors.at(5);
    freshWaterLevelLightSensor = m_maintenanceLightSensors.at(6);

    connect(m_pumpControl, &PumpControl::targetPressureReached, this, &Maintenance::onTargetPressureReached);
    connect(m_simulation, &Simulation::waterDispensed, this, &Maintenance::onFlushSystem);
    connect(m_simulation, &Simulation::notEnoughWater, this, &Maintenance::refillWater);
    connect(m_simulation, &Simulation::notEnoughBeans, this, &Maintenance::checkCremaBeansLevel);
    connect(m_simulation, &Simulation::notEnoughBeans, this, &Maintenance::checkEspressoBeansLevel);
    connect(m_simulation, &Simulation::notEnoughMilk, this, &Maintenance::checkMilkLevel);


    connect(m_simulation, &Simulation::waterDispensed, waitOnFlush, &QEventLoop::quit);

}

Maintenance::~Maintenance() {
    delete cremaBeanLevelLightSensor;
    delete espressoBeanLevelLightSensor;
    delete milkLevelLightSensor;
    delete dirtyWaterLightSensor;
    delete wasteDisposaleLightSensor;
    delete cupInsertedLightSensor;
    delete freshWaterLevelLightSensor;
}

void Maintenance::shutdown(){

}

void Maintenance::fullMaintenanceSchedule(){
    qDebug() << "Maintenance::fullMaintenanceSchedule currentState: " << (int)m_coffeeStateMachine->getCurrentState();
    flushTheSystem();
    qDebug() << "Maintenance::fullMaintenanceSchedule system flushed";

    qDebug() << "Maintenance::fullMaintenanceSchedule waiting for continue signal...";
    waitOnFlush->exec();  // hier wird gewartet

    refillWater();
    qDebug() << "Maintenance::fullMaintenanceSchedule water refilled";
    checkCremaBeansLevel();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked beans";
    checkEspressoBeansLevel();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked espresso beans";
    checkMilkLevel();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked milk level";
    checkWasteDisposal();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked waste disposal";
    checkMilkTemperature();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked milk temperature";
    checkThermoblockTemperature();
    qDebug() << "Maintenance::fullMaintenanceSchedule checked water temperature";


    qDebug() << "Maintenance::fullMaintenanceSchedule currentState: " << (int)m_coffeeStateMachine->getCurrentState();
    emit maintenanceCheckComplete();
}

vector<issues> Maintenance::getOpenIssues(){
    return this->openIssues;
}

void Maintenance::refillWater(){
    m_waterValve->setValveState(valveState::closed);
    m_freshWaterSupplyValve->setValveState(valveState::open);

    refillWaterTimer = new QTimer(this);
    connect(refillWaterTimer, &QTimer::timeout, this, [this](){
        if(freshWaterLevelLightSensor->detectLight() == detection::lightBlocked){
            this->refillWaterTimer->stop();
            emit waterRefilled();
            m_freshWaterSupplyValve->setValveState(valveState::closed);
        }
    });
    refillWaterTimer->start(1000);
}

void Maintenance::flushTheSystem(){
    m_pumpControl->setTargetPressure(3000);
    m_pumpControl->setFlushSystem(true);
}

void Maintenance::onTargetPressureReached(){
    if(m_pumpControl->getFlushSystem()){
        m_waterValve->setValveState(valveState::open);
        m_simulation->pumpWater(30, 10); //30 ml with 10ml /s flushen
    }
}

void Maintenance::onFlushSystem(){
    m_pumpControl->setFlushSystem(false);
    m_pumpControl->setPressureReached(false);
    m_waterValve->setValveState(valveState::closed);

}



bool Maintenance::checkDirtyWater(){
    if(dirtyWaterLightSensor->detectLight() == detection::lightBlocked){
        openIssues.push_back(issues::dirtyWater);
        return false;
    }else{
        return true;
    }
}

bool Maintenance::checkCremaBeansLevel(){
    if(cremaBeanLevelLightSensor->detectLight() == detection::lightReceived){
        openIssues.push_back(issues::cremaBeans);
        return false;
    }else{
        return true;
    }
}

bool Maintenance::checkEspressoBeansLevel(){
    if(cremaBeanLevelLightSensor->detectLight() == detection::lightReceived){
        openIssues.push_back(issues::espressoBeans);
        return false;
    }else{
        return true;
    }
}

bool Maintenance::checkMilkLevel(){
    if(milkLevelLightSensor->detectLight() == detection::lightReceived){
        openIssues.push_back(issues::milkEmpty);
        return false;
    }else{
        return true;
    }
}

bool Maintenance::checkMilkTemperature(){
    if(m_milkUnit->getMilkToWarm() >= 30){
        qDebug() << "Maintenance::checkMilkToWarm Milk was too warm too long and"
                    " needs to be changed.";
        this->openIssues.push_back(issues::milkChange);
        return false;
    } else {
        qDebug() << "Maintenance::checkMilkToWarm Milk Temperature is okay";
        return true;
    }
}

void onMilkNeedsToBeCooled(); // TODO

bool Maintenance::checkWasteDisposal(){
    if(wasteDisposaleLightSensor->detectLight() == detection::lightBlocked){
        openIssues.push_back(issues::wasteDisposal);
        return false;
    }else{
        return true;
    }
}

bool Maintenance::checkThermoblockTemperature(){
    int temperatureControl = m_simulation->getCurrentTemperature();
    if(temperatureControl > 60 && temperatureControl < 125){
        qDebug() << "Thermoblocktemperatur is good, setting mode to standby cool";
        return true;
    }else if(temperatureControl > 150){
        qDebug() << "Thermoblock is too hot, setting mode to standby cool";
        m_thermoblock->setTargetTemperature(mode::standby);
        return false;
    }else {
        qDebug() << "checkThermoblockTemperature default, setting mode to standby cool";
        return true;
        m_thermoblock->setTargetTemperature(mode::standby);
        return false;
    }
}


// TODO error case for coin stuck
