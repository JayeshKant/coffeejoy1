#include "preparationshutdown.h"


PreparationShutdown::PreparationShutdown (
    CoinChecker* m_coinChecker,
    array<LightSensor*, 9> m_lightSensors,
    Simulation* m_simulation,
    Maintenance* m_maintenance,
    QObject* parent) :
        QObject(parent),
        m_coinChecker(m_coinChecker),
        m_lightSensors(m_lightSensors),
        m_simulation(m_simulation),
        m_maintenance(m_maintenance){}

PreparationShutdown::~PreparationShutdown() {}

void PreparationShutdown::shutdown() {
    m_simulation->shutdown();
    m_coinChecker->shutdown();
    for (LightSensor* lightSensor : m_lightSensors) {
        lightSensor->shutdown();
    }
    m_maintenance->fullMaintenanceSchedule();
}
