#pragma once

#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

#include "pumpcontrol.h"
#include "thermoblock.h"
#include "lightsensor.h"
#include "coffeestatemachine.h"
#include "simulation.h"
#include "pumpcontrol.h"
#include "valve.h"
#include "pump.h"
#include "milkunit.h"

#include <vector>
using std::vector;

enum class maintenanceStates{full, empty};
enum class maintenanceInstructions{clean, refill};
enum class issues{dirtyWater, milkEmpty, milkChange, wasteDisposal, cremaBeans, espressoBeans, noWater};


class Maintenance : public QObject {
    Q_OBJECT

public:

    Maintenance(CoffeeStateMachine* m_coffeeStateMachine,
                Simulation* m_simulation,
                PumpControl* m_pumpControl,
                Pump* m_waterPump,
                Valve* m_waterValve,
                Thermoblock* m_thermoblock,
                Valve* m_milkValve,
                array<LightSensor*,7> m_maintenanceLightSensor,
                Valve* m_freshWaterSupplyValve,
                MilkUnit* m_milkUnit,
                QObject* parent = nullptr);
    ~Maintenance();


    void fullMaintenanceSchedule();

    void shutdown();

    vector<issues> getOpenIssues();

signals:
    void waterRefilled();

private slots:
    void onFlushSystem();
    void onTargetPressureReached();

private:

    void flushTheSystem();
    void refillWater();



    bool checkDirtyWater();
    bool checkCremaBeansLevel();
    bool checkEspressoBeansLevel();
    bool checkMilkLevel();
    bool checkWasteDisposal();

    bool checkMilkTemperature();
    bool checkThermoblockTemperature();

    PumpControl* m_pumpControl;
    Simulation* m_simulation;
    CoffeeStateMachine* m_coffeeStateMachine;
    Valve* m_waterValve;
    Valve* m_milkValve;
    Valve* m_freshWaterSupplyValve;
    Pump* m_waterPump;
    Thermoblock* m_thermoblock;
    MilkUnit* m_milkUnit;

    QEventLoop* waitOnFlush;
    //QTimer* checkMilkTemperatureTimer;
    QTimer* refillWaterTimer = new QTimer(this);

    LightSensor* cremaBeanLevelLightSensor;
    LightSensor* espressoBeanLevelLightSensor;
    LightSensor* milkLevelLightSensor;
    LightSensor* dirtyWaterLightSensor;
    LightSensor* wasteDisposaleLightSensor;
    LightSensor* cupInsertedLightSensor;
    LightSensor* freshWaterLevelLightSensor;


    vector<issues> openIssues;

protected:


};

