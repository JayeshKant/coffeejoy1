#pragma once

#include <QObject>
#include <QDebug>

#include "lightsensor.h"
#include "coinchecker.h"
#include "simulation.h"
#include "maintenance.h"
#include "touchhandler.h"

using std::array;

class PreparationShutdown : public QObject {
    Q_OBJECT
public:
    PreparationShutdown(
        CoinChecker* m_coinChecker,
        array<LightSensor*, 9> m_lightSensors,
        Simulation* m_simulation,
        Maintenance* m_maintenance,
        TouchHandler* m_touchHandler,
        QObject* parent = nullptr);
    ~PreparationShutdown();
    void shutdown();

private:
    CoinChecker* m_coinChecker;
    array<LightSensor*, 9> m_lightSensors;
    Simulation* m_simulation;
    Maintenance* m_maintenance;
    TouchHandler* m_touchHandler;
};
