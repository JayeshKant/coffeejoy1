#include "unittest_coinchecker.h"

#include <iostream>

using namespace std;

void TestCoinChecker::init()
{
    // Create mock CoinSensor
    mockCoinSensor = new CoinSensorMock();
    QVERIFY(mockCoinSensor != nullptr);

    // Create SUT with mock dependency
    sut = new CoinChecker(mockCoinSensor);
    QVERIFY(sut != nullptr);
}

void TestCoinChecker::cleanup()
{
    delete sut;
    sut = nullptr;
    delete mockCoinSensor;
    mockCoinSensor = nullptr;
}

// Test constructor creates valid object and getCoinSensor() returns correct sensor
void TestCoinChecker::testConstructorAndGetter()
{
    // Exercise the SUT - verify that constructor worked and getter returns correct object
    CoinSensor* retrievedSensor = sut->getCoinSensor();

    // Check end results
    QVERIFY(retrievedSensor != nullptr);
    QCOMPARE(retrievedSensor, mockCoinSensor);
}

QTEST_APPLESS_MAIN(TestCoinChecker);
