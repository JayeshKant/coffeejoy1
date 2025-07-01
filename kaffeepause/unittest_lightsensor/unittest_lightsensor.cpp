#include "unittest_lightsensor.h"

using namespace std;

void TestLightSensor::init()
{
    // Create mock objects
    mockSimulation = new SimulationMock();
    mockStateMachine = new CoffeeStateMachineMock();

    // Create SUT with mock dependencies
    sut = new TestableLightSensor(mockSimulation, lightSensors::cupInserted);
    sut->setCoffeeStateMachine(mockStateMachine);
}

void TestLightSensor::cleanup()
{
    delete sut;
    sut = nullptr;
    delete mockSimulation;
    mockSimulation = nullptr;
    delete mockStateMachine;
    mockStateMachine = nullptr;
}

// Test that reset() function sets all values to their default state
void TestLightSensor::testReset()
{
    sut->detectedCoinObject(detection::lightBlocked); // This will change internal state

    // Exercise the SUT - reset should restore default values
    sut->reset();

    // Check that reset() sets all values to their default state
    QCOMPARE(sut->getDetectionCounter(), 0);
    QCOMPARE(sut->getVerifyDetection(), false);
    QCOMPARE(sut->getStatusBegin(), detection::lightReceived);
    QCOMPARE(sut->getDetectChangeInCup(), false);
}

QTEST_APPLESS_MAIN(TestLightSensor);
