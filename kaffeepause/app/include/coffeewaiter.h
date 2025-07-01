#pragma once

#include <QObject>
#include <QDebug>

#include "thermoblock.h"
#include "lightsensor.h"
#include "payment.h"

class Simulation;
class Grinder;
class CoffeeSelection;
class CoffeeStateMachine;
class LightSensor;

class CoffeeWaiter : public QObject
{
    Q_OBJECT

public:

    CoffeeWaiter(
        Payment* m_payment,
        LightSensor* m_cupInserted,
        Thermoblock* m_thermoblock,
        Simulation* m_simulation,
        CoffeeStateMachine* m_coffeeStateMachine,
        CoffeeSelection* m_coffeeSelection,
        QObject* parent = nullptr);
    ~CoffeeWaiter();

    void reset();

    void setGrinder(Grinder* m_grinder);
    // void setBrewingUnit(BrewingUnit* m_brewingUnit);

    void setAmountWater();
    int getAmountWater();

    int getAmountMilk();
    void setAmountMilk();

    virtual int getAmountBeans();
    void setBeans();

    virtual beans getTypeBeans();
    coffee getTypeCoffee();

    void makeCoffee();
    bool insertCup();

    void startCoffee();

    void heatUpWater(mode thermoBlockMode);
    void dispenseCoffee();


signals:
    void grindCoffee();
    void startBrewing();

private slots:
    void onWaterTemperatureOK();
    void onCupInserted();

private:
    CoffeeSelection* m_coffeeSelection;
    CoffeeStateMachine* m_coffeeStateMachine;
    Simulation* m_simulation;
    Thermoblock* m_thermoblock;
    Grinder* m_grinder;
    Payment* m_payment;
    LightSensor* m_cupInserted;

    intensity coffeeIntensity = intensity::medium;
    coffee typeCoffee = coffee::undefined;
    beans typeBeans = beans::espresso;

    int amountWater = 0; //in ml
    int amountMilk = 0; //in ml
    int amountBeans = 0; //in g
};

