#include "unittest_grinder.h"

#include <iostream>

using namespace std;

void TestGrinder::init()
{
    // Create mock objects
    mockStateMachine = new CoffeeStateMachineMock();
    mockWaiter = new CoffeeWaiterMock();
    mockSimulation = new SimulationMock();

    // Create SUT with mock dependencies
    sut = new Grinder(mockStateMachine, mockWaiter, mockSimulation);

    // Verify initial state
    QVERIFY(sut->getGrindRate() == 4);
    QVERIFY(sut->getAmountBeans() == 0);
    QVERIFY(sut->getTypeBeans() == beans::espresso);
}

void TestGrinder::cleanup()
{
    delete sut;
    sut = nullptr;
    delete mockStateMachine;
    mockStateMachine = nullptr;
    delete mockWaiter;
    mockWaiter = nullptr;
    delete mockSimulation;
    mockSimulation = nullptr;
}

// Test that reset() function sets all values to their default state
void TestGrinder::testReset()
{
    // Exercise the SUT - reset should restore default values
    sut->reset();

    // Check end results
    QCOMPARE(sut->getGrindRate(), 4);
    QCOMPARE(sut->getAmountBeans(), 0);
    QCOMPARE(sut->getTypeBeans(), beans::espresso);
}

QTEST_APPLESS_MAIN(TestGrinder);
