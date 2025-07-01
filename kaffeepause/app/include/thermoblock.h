#pragma once

#include <QObject>
#include <QTimer>

class Simulation;

enum class mode { standby = 0, brewing = 1, steaming = 2, maintenance = 3};

class Thermoblock : public QObject {
    Q_OBJECT
public:
    Thermoblock(Simulation* m_simulation, QObject* parent= nullptr);
    ~Thermoblock();

    void start();
    void reset();

    void setCurrentMode(mode newMode);
    mode getCurrentMode();
    void setTargetTemperature(mode newMode);
    int getTargetTemperature();
    bool isAtTargetTemperature();
    void controlHeating();


signals:
    void waterTemperatureOK();
    void steamingOK();
    void temperatureChanged(int temp);

private:
    Simulation* m_simulation;

    QTimer* timer;

    int targetTemperature = 0;
    mode currentMode = mode::standby;

};
