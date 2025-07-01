#pragma once

#include <QObject>
#include <QDebug>

#include "lightsensor.h"

using std::array;

class PreparationShutdown : public QObject {
    Q_OBJECT
public:
    PreparationShutdown(
        CoinChecker* m_coinChecker,
        array<LightSensor*, 9> m_lightSensors,

        QObject* parent = nullptr);
    ~PreparationShutdown();
    void shutdown();

private:
    CoinChecker* m_coinChecker;
    array<LightSensor*, 9> m_lightSensors;

};
