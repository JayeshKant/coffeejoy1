#pragma once

#include <QTimer>
#include <QObject>
#include <QDebug>

#include "coinsupply.h"
#include "payment.h"
#include "lightsensor.h"
#include "coffeewaiter.h"
#include "thermoblock.h"
#include "pump.h"
#include "touchscreen.h"
#include "buttonTypes.h"
#include "valve.h"

#include <QRandomGenerator>

using std::map;

enum class statusOfBlocker {
    closed = 0,
    opened = 1
};


class CoinSensor;
class CoinSupply;
class PumpControl;
class TouchHandler;

class Simulation : public QObject{
    Q_OBJECT
public:
    Simulation(TouchScreen* m_touchScreen, CoinSensor* m_coinSensor,
               Pump* m_pump, Valve* m_freshWaterSupplyValve, QObject *parent = nullptr);
    ~Simulation();

    void start();
    void reset();
    void shutdown();

    void setPumpControl(PumpControl* m_pumpControl);
    void setTouchHandler(TouchHandler* m_touchHandler);

    // For GUI

    void onEurButtonClicked(coinType buttonClicked);
    void onButtonTouched(buttons clickedButtonType);
    void cupInserted(bool status);

    // For CoinSupply
    void addCoin(coinType, int width);
    void removeCoin(coinType, int width);
    void setCoinLevel(coinType, int levelInMicrometer);
    virtual int getCoinLevel(coinType);

    void setCoinReturn(coinType);
    int getCoinReturn(coinType);

    void setCoinFlap(coinDestination destination);
    std::map<coinType, int> getCoinLevelMap();
    statusOfBlocker getStatusOfBlocker(coinDestination);
    void closeAllCoinSupplyBlocker();

    // For change
    void addToCoinReturnMap(coinType);
    void setEnoughChange(bool enoughChange);
    bool getEnoughChange();
    std::map<coinType, int> getCoinReturnMap();

    //For LightSensor
    void updateLightSensors();
    void setCoinLightSensorInSimulation(lightSensors, LightSensor*);
    virtual detection getDetection(lightSensors sensor);
    void setDetection(lightSensors sensor, detection newValue);

    void toggleDetection(enum lightSensors sensor);


    // For CoffeeBeans
    void grindBeans(int grindRate, beans typeBeans, int amount);
    void reduceBeans();
    void resetCoffeeBeansLevel(beans typeBeans);
    int getCoffeeBeansLevel(beans typeBeans);


    // For Thermoblock
    void setCurrentThermoblockMode(mode newMode);
    mode getCurrentThermoblockMode();

    void setCurrentTemperature(int temperature);
    int getCurrentTemperature();


    // For Brewing
    void pumpWater(int waterNeeded, int flowRate);
    void reduceWater();
    int getCurrentWaterAmount();
    int getBeansGrinded();
    int getWaterDispensed();
    int getMaxWaterAmount();

    // For PumpControl

    void setCurrentPressure(int newPressure);
    int getCurrentPressure();
    void updatePressure();

    // For Milk
    void pumpMilk(int milkNeeded, int flowRateMilk, int flowRateWater);
    void reduceMilk();
    int getCurrentMilkAmount();
    void setMilkTemperature(int milkTemperature);
    int getMilkTemperature();
    void heatMilk();
    int getDispencedAmountMilk();

    // For Water
    void fillFreshWaterValve();

    // Waste
    void setGrindedWaste(int wasteAmount);
    int getGrindedWaste();

signals:
    void notEnoughWater(); //TODO implement in maintenace / coffeestatemachine
    void notEnoughMilk(); //TODO implement in maintenace / coffeestatemachine
    void notEnoughBeans(); //TODO implement in maintenace / coffeestatemachine
    void pressureToHigh(); //TODO implement in maintenace / coffeestatemachine
    void waterDispensed();
    void milkDispensed();
    void grindedBeans();
    void grindingProgress(int gramsGround, int totalRequired);
    void milkTemperatureChanged(int temperature);
    void pressureUpdated(int currentPressure);
    void buttonTouched();
    void coinInserted(CoinData coinClicked);

public slots:
    void onWaitingForDetectedCoin(lightSensors);


private:
    TouchScreen* m_touchScreen;
    TouchHandler* m_touchHandler;
    CoinSensor* m_coinSensor;
    PumpControl* m_pumpControl;
    Pump* m_pump;
    CoffeeStateMachine* m_stateMachine;

    QTimer* pressureTimer;
    QTimer* pumpWaterTimer;
    QTimer* pumpMilkTimer;
    QTimer* grinderTimer;
    QTimer* updateLightSensorTimer;
    QTimer* updateMilkHeatingTimer;
    QTimer* fillFreshWaterValveTimer;
    QTimer* pollCoffeeBeanWasteTimer;

    //Maps for Coins
    map<coinType, int> coinLevelMap;
    map<coinType, int> coinReturnMap;
    statusOfBlocker cashRegisterBlocker, hopperBlocker;

    //Change
    bool enoughChange = true;

    //Maps for LightSensors
    map<lightSensors, detection> lightSensorsStatusMap;
    map<lightSensors, LightSensor*> m_coinLightSensors;

    //Map for CoffeeBeans
    const int maxCoffeeBeansInGramm = 2000;
    map<beans, int> beansLevelMap;

    beans selectedBeans = beans::espresso;
    int requiredAmountBeans = 0;
    int grindRate = 0;
    int beansGrinded = 0;

    //Thermoblock

    int currentTemperature = 20;
    int waterPumped = 0;
    int flowRate = 0;

    const int maxWaterAmount = 500;
    int currentWaterAmount = maxWaterAmount; // max 200ml
    int waterInMlDispensed = 0;
    int waterNeeded = 0;

    //MilkUnit

    const int maxAmountMilkInMl = 5000;
    int currentMilkAmount = maxAmountMilkInMl; // max 5000ml
    int milkInMlDispensed = 0;
    int milkNeeded = 0;
    int flowRateWater = 0;
    int flowRateMilk = 0;
    int milkTemperature = 25;

    // Valve
    Valve* m_freshWaterSupplyValve;

    //PumpControl

    int currentPressure = 0; // when starting always 0 in mbar

    //Waste

    const int maxWasteWaterInML = 3000;
    int wasteWater = 0;
    int grindedWaste = 0;

};
