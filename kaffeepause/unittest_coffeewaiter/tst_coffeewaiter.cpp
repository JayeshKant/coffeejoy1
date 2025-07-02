#include <QTest>

#include "simulation.h"
#include "coffeeselection.h"
#include "coffeestatemachine.h"
#include "coffeewaiter.h"
#include "thermoblock.h"
#include "lightsensor.h"
#include "payment.h"
#include "dummymodules.h"

class CoffeeWaiterTest : public QObject
{
    Q_OBJECT

public:
    CoffeeWaiterTest();
    ~CoffeeWaiterTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSetBeansData();
    void testSetBeans();

private:
    Simulation* simulation;
    Payment* payment;
    Thermoblock* thermoblock;
    CoffeeSelection* coffeeSelection;
    CoffeeStateMachine* coffeeStateMachine;
    LightSensor* cupInsertedSensor;
    CoffeeWaiter* coffeeWaiter;
};

CoffeeWaiterTest::CoffeeWaiterTest() {}

CoffeeWaiterTest::~CoffeeWaiterTest() {}

void CoffeeWaiterTest::initTestCase() {
    simulation = new Simulation(
        new DummyTouchScreen(this),
        new DummyCoinSensor(this),
        new DummyPump(this),
        new DummyValve(valveType::supply, this),
        this
        );

    payment = new Payment(
        simulation,
        // new DummySimulation(this),
        new DummyCoffeeSelection(this),
        this
        );

    thermoblock = new Thermoblock(
        simulation,
        // new DummySimulation(this),
        this
        );

    coffeeSelection = new CoffeeSelection(this);

    coffeeStateMachine = new CoffeeStateMachine(
        simulation,
        payment,/*
        new DummySimulation(this),
        new DummyPayment(this),*/
        new DummyCoinChecker(this),
        coffeeSelection,
        // new DummyCoffeeSelection(this),
        new DummyCoinSupply(this),
        this
        );

    cupInsertedSensor = new LightSensor(
        simulation,
        lightSensors::cupInserted,
        this
        );

    coffeeWaiter = new CoffeeWaiter(
        payment,
        cupInsertedSensor,
        thermoblock,
        simulation,
        coffeeStateMachine,
        coffeeSelection,
        // new DummyPayment(this),
        // new DummyLightSensor(this),
        // new DummyThermoblock(this),
        // new DummySimulation(this),
        // new DummyCoffeeStateMachine(this),
        // new DummyCoffeeSelection(this),
        this
        );

}

void CoffeeWaiterTest::cleanupTestCase() {
    delete coffeeWaiter;
}

void CoffeeWaiterTest::testSetBeansData() {
    QTest::addColumn<coffee>("typeCoffee");
    QTest::addColumn<intensity>("intensity");
    QTest::addColumn<int>("expectedAmount");
    QTest::addColumn<beans>("expectedBeans");

    QTest::newRow("espresso-medium") << coffee::espresso << intensity::medium << 16 << beans::espresso;
    QTest::newRow("espresso-strong") << coffee::espresso << intensity::strong << 20 << beans::espresso;
    QTest::newRow("espresso-light") << coffee::espresso << intensity::light << 8 << beans::espresso;

    QTest::newRow("crema-light") << coffee::crema << intensity::light << 8 << beans::crema;
    QTest::newRow("crema-medium") << coffee::crema << intensity::medium << 12 << beans::crema;
    QTest::newRow("crema-strong") << coffee::crema << intensity::strong << 16 << beans::crema;

    QTest::newRow("cappuccino-strong") << coffee::cappuccino << intensity::strong << 20 << beans::espresso;
}

void CoffeeWaiterTest::testSetBeans() {
    QFETCH(coffee, typeCoffee);
    QFETCH(enum intensity, intensity);
    QFETCH(int, expectedAmount);
    QFETCH(beans, expectedBeans);

    coffeeWaiter->setTypeCoffee(typeCoffee);
    coffeeWaiter->setCoffeeIntensity(intensity);
    coffeeWaiter->setBeans();

    QCOMPARE(coffeeWaiter->getAmountBeans(), expectedAmount);
    QCOMPARE(coffeeWaiter->getTypeBeans(), expectedBeans);

}

QTEST_APPLESS_MAIN(CoffeeWaiterTest)

#include "tst_coffeewaiter.moc"
