#include "preparationshutdown.h"
#include <QDebug>

PreparationShutdown::PreparationShutdown(
    CoinChecker* m_coinChecker,
    array<LightSensor*, 9> m_lightSensors,

    QObject* parent) {}

PreparationShutdown::~PreparationShutdown() {}

void PreparationShutdown::shutdown() {}
