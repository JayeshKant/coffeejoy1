#pragma once

#include <QObject>
#include <QtTest>

#include "lightsensor.h"
#include "simulation.h"
#include "coffeestatemachine.h"
#include "payment.h"
#include "coinchecker.h"
#include "coinsupply.h"
#include "coinsensor.h"

// Mock classes for LightSensor dependencies
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

    // Mock implementations for testing
    detection mockDetection = detection::lightReceived;

    detection getDetection(lightSensors sensor) override {
        return mockDetection;
    }

    int getCoinLevel(coinType type) override {
        return 100; // Mock value
    }

    // Helper method to set mock detection for tests
    void setMockDetection(detection det) {
        mockDetection = det;
    }
};

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

    state mockCurrentState = state::waitingForCup;

    state getCurrentState() override {
        return mockCurrentState;
    }

    void setMockCurrentState(state s) {
        mockCurrentState = s;
    }
};

// Extended LightSensor class
class TestableLightSensor : public LightSensor
{
    Q_OBJECT
public:
    TestableLightSensor(Simulation* simulation, lightSensors sensor, QObject* parent = nullptr)
        : LightSensor(simulation, sensor, parent) {}

};

// Unit Test class for LightSensor
class TestLightSensor : public QObject
{
    Q_OBJECT

    TestableLightSensor* sut;
    SimulationMock* mockSimulation;
    CoffeeStateMachineMock* mockStateMachine;

public:
    explicit TestLightSensor(QObject *parent = nullptr) : QObject(parent) {}

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
