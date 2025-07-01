#pragma once

#include "payment.h"


#include <QObject>
#include <QString>
#include <QTimer>

//using std::string;

class Simulation;
class BrewingUnit;
class MilkUnit;
class CoffeeStateMachine;

enum detection {lightBlocked = 1, lightReceived = 2};


enum lightSensors {
    hopper = 0,
    coinSupply = 1,
    espressoBeanLevel = 2,
    cremaBeanLevel = 3,
    wasteDisposale = 4,
    cupInserted = 5,
    milkLevel = 6,
    dirtyWater = 7,
    freshWaterLevel =8
};

class LightSensor : public QObject
{
    Q_OBJECT
public:
    LightSensor(Simulation* m_simulation, lightSensors sensor, QObject* parent = nullptr);
    ~LightSensor();

    void setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine);

    void start();
    void reset();
    void shutdown();

    enum lightSensors getSensorName();

    void detectedCoinObject(detection statusBegin);
    int detectLevelInMicrometer(coinType);
    detection detectLight();

    void searchCup();
    void pollDetectCoin();

    // Getter methods for testing
    int getDetectionCounter() const { return detectionCounter; }
    bool getVerifyDetection() const { return verifydetection; }
    detection getStatusBegin() const { return statusBegin; }
    bool getDetectChangeInCup() const { return detectChangeInCup; }

signals:
    void timeoutError(QString name);
    void waitingForDetectedCoin(lightSensors);
    void cupInserted();
    void cupTaken();

    void coinDetectionSuccess(bool);


private slots:

    void getDetectionFromSimulation();

private:

    QTimer* waitingForCup;
    QTimer* pollLightSensor;
    QTimer* pollDetection;
    QTimer* timeOutLimit;

    Simulation* m_simulation;
    BrewingUnit* m_brewingUnit;
    MilkUnit* m_milkUnit;
    CoffeeStateMachine* m_coffeeStateMachine;

    detection statusLightSensor;
    lightSensors sensor;

    int detectionCounter = 0;
    bool verifydetection = false;
    detection statusBegin = lightReceived;
    bool detectChangeInCup = false;


};

