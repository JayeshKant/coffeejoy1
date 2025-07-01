#pragma once

#include "coffeewaiter.h"
#include "simulation.h"
#include "coffeestatemachine.h"

#include <QObject>
#include <QDebug>

class Grinder : public QObject
{
    Q_OBJECT

public:

    Grinder(CoffeeStateMachine* m_coffeeStateMachine, CoffeeWaiter* m_coffeeWaiter, Simulation* m_simulation, QObject* parent = nullptr);
    ~Grinder();
    void grind();
    void setBeansInformation();
    void reset();

    // Getter methods for testing
    int getGrindRate() const { return grindRate; }
    int getAmountBeans() const { return amountBeans; }
    beans getTypeBeans() const { return typeBeans; }

signals:
    void grindingComplete();

private slots:
    void onGrindedBeans();

private:
    CoffeeWaiter* m_coffeeWaiter;
    Simulation* m_simulation;
    CoffeeStateMachine* m_coffeeStateMachine;

    int grindRate = 4;
    int amountBeans = 0;
    beans typeBeans = beans::espresso;

    void grindBeans();

protected:

};

