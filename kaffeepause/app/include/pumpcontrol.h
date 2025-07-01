#pragma once

#include <QObject>

#include "pump.h"
#include "simulation.h"

class PumpControl : public QObject
{
    Q_OBJECT
public:
    PumpControl(Simulation* m_simulation, Pump* m_pump, QObject* parent = nullptr);
    ~PumpControl();

    void start();
    void reset();

    void startPump();
    void stopPump();
    void checkCurrentPressure();

    int getTargetPressure();
    void setTargetPressure(int targetPressure);

    void setBrewCoffee(bool brewingCoffee);
    void setMilkDispense(bool milkDispense);
    void setPressureReached(bool pressureReached);

signals:
    void targetPressureReached();

private:
    Pump* m_pump;
    Simulation* m_simulation;

    QTimer* pressureTimer;


    int targetPressure = 9000; //in mbar
    bool brewingCoffee = false;
    bool milkDispense = false;
    bool pressureReached = false;

};
