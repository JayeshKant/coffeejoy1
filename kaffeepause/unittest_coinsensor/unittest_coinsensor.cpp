#include "unittest_coinsensor.h"

#include <iostream>

using namespace std;

void TestCoinSensor::init()
{
    sut = new CoinSensor();
    QVERIFY(sut != nullptr);
}

void TestCoinSensor::cleanup()
{
    delete sut;
    sut = nullptr;
}

// Test that resetValues() correctly resets coinIsValid to false and detectedCoinType to RESET
void TestCoinSensor::testResetValues()
{
    // First, set some values to non-default state
    sut->setCoinIsValid(true);
    sut->setCoinTypeFromCoinData({2125, 167, 392, 0}); // Valid EUR005 coin data

    // Verify that values were changed from defaults
    QVERIFY(sut->getCoinIsValid() == true);
    QVERIFY(sut->getCoinType() != coinType::RESET);

    // Exercise the SUT - call resetValues()
    sut->resetValues();

    // Check that values are reset to defaults
    QVERIFY(sut->getCoinIsValid() == false);
    QVERIFY(sut->getCoinType() == coinType::RESET);
}

QTEST_APPLESS_MAIN(TestCoinSensor);
