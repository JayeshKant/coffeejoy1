#pragma once

#include <QObject>
#include <QtTest>

#include "grinder.h"
#include "coffeetypes.h"
#include "coffeestatemachine.h"
#include "coffeewaiter.h"
#include "simulation.h"

class CoffeeStateMachineMock : public CoffeeStateMachine
{
    Q_OBJECT
public:
    // Create a minimal mock that bypasses the complex constructor
    CoffeeStateMachineMock() : CoffeeStateMachine(
        nullptr, // simulation
        nullptr, // payment
        nullptr, // coinChecker
        nullptr, // coffeeSelection
        nullptr  // coinSupply
    ) {}
    virtual ~CoffeeStateMachineMock() = default;
};

class CoffeeWaiterMock : public CoffeeWaiter
{
    Q_OBJECT
public:
    // Create a minimal mock that bypasses the complex constructor
    CoffeeWaiterMock() : CoffeeWaiter(
        nullptr, // payment
        nullptr, // cupInserted
        nullptr, // thermoblock
        nullptr, // simulation
        nullptr, // coffeeStateMachine
        nullptr  // coffeeSelection
    ) {}
    virtual ~CoffeeWaiterMock() = default;

    // Override methods with mock implementations
    int getAmountBeans() override { return 10; }
    beans getTypeBeans() override { return beans::espresso; }
};

class SimulationMock : public Simulation
{
    Q_OBJECT
public:
    // Create a minimal mock that bypasses the complex constructor
    SimulationMock() : Simulation(
        nullptr, // touchScreen
        nullptr, // coinSensor
        nullptr, // pump
        nullptr  // freshWaterSupplyValve
    ) {}
    virtual ~SimulationMock() = default;
};

// Unit Test class for Grinder
class TestGrinder : public QObject
{
    Q_OBJECT

    Grinder* sut;
    CoffeeStateMachineMock* mockStateMachine;
    CoffeeWaiterMock* mockWaiter;
    SimulationMock* mockSimulation;

public:
    explicit TestGrinder(QObject *parent = nullptr) : QObject(parent) {}

private slots:

    // Executed by QtTest before and after test suite
    void initTestCase() {}
    void cleanupTestCase() {}

    // Executed by QtTest before and after each test
    void init();
    void cleanup();

    // tests
    void testReset();
};
