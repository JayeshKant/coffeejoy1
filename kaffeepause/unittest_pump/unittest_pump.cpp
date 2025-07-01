#include "unittest_pump.h"

void TestPump::init()
{
    sut = new Pump();
}

void TestPump::cleanup()
{
    delete sut;
    sut = nullptr;
}

// Test that the pump is initially not running
void TestPump::testInitialState()
{
    // Check that pump is not running after construction
    QVERIFY(!sut->isRunning());
}

QTEST_APPLESS_MAIN(TestPump);
