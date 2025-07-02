#include "unittest_coffeeselection.h"


using namespace std;

void TestCoffeeSelection::init()
{
    sut = new CoffeeSelection();
    QVERIFY(sut != nullptr);
}

void TestCoffeeSelection::cleanup()
{
    delete sut;
    sut = nullptr;
}

// Test that getTypeCoffee() and setTypeCoffee() work correctly
void TestCoffeeSelection::testGetSetTypeCoffee()
{
    // Test initial/default value
    QVERIFY(sut->getTypeCoffee() == coffee::undefined);

    // Test setting and getting crema
    sut->setTypeCoffee(coffee::crema);
    QVERIFY(sut->getTypeCoffee() == coffee::crema);

    // Test setting and getting espresso
    sut->setTypeCoffee(coffee::espresso);
    QVERIFY(sut->getTypeCoffee() == coffee::espresso);

    // Test setting and getting cappuccino
    sut->setTypeCoffee(coffee::cappuccino);
    QVERIFY(sut->getTypeCoffee() == coffee::cappuccino);

    // Test setting back to undefined
    sut->setTypeCoffee(coffee::undefined);
    QVERIFY(sut->getTypeCoffee() == coffee::undefined);
}

QTEST_APPLESS_MAIN(TestCoffeeSelection);
