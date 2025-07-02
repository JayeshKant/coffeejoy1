#include <QTest>
#include <QCoreApplication>
#include <QSignalSpy>

#include "touchhandler.h"
#include "touchscreen.h"
#include "coinsupply.h"
#include "simulation.h"
#include "grinder.h"
#include "thermoblock.h"
#include "brewingunit.h"
#include "pumpcontrol.h"
#include "valve.h"
#include "pump.h"
#include "milkunit.h"
#include "payment.h"
#include "coffeeselection.h"
#include "maintenance.h"
#include "startupmanager.h"
#include "preparationshutdown.h"


class systemtest : public QObject
{
    Q_OBJECT

public:
    systemtest();
    ~systemtest();

    void initTestCase();

private slots:
    void testEspressoMedium(); //Testing Change
    void testCremaMedium();
    void testCapuccinoMedium();

private:
    // Direkt enthaltene Klassen
    CoffeeSelection* coffeeSelection;
    CoinSensor* insertCoinSensor;
    CoinChecker* coinChecker;
    Pump* pump;
    Pump* milkPump;
    Pump* waterPump;
    TouchScreen* touchScreen;

    Valve* waterValve;
    Valve* steamValve;
    Valve* milkValve;
    Valve* freshWaterSupplyValve;
    std::array<Valve*, 4> valves;

    Simulation* simulation;
    Payment* payment;

    std::array<CoinSensor*, 6> coinSensors;
    std::array<LightSensor*, 2> coinLightSensors;
    std::array<LightSensor*, 7> maintenanceLightSensors;
    std::array<LightSensor*, 9> lightSensors;

    CoinSupply* coinSupply;

    CoffeeStateMachine* coffeeStateMachine;
    TouchHandler* touchHandler;
    Thermoblock* thermoblock;
    CoffeeWaiter* coffeeWaiter;
    Grinder* grinder;
    PumpControl* pumpControl;
    BrewingUnit* brewingUnit;
    MilkUnit* milkUnit;
    Maintenance* maintenance;
    StartUpManager* startUpManager;
    PreparationShutdown* preparationShutdown;

};

systemtest::systemtest() {
    initTestCase();
}

systemtest::~systemtest() {}

void systemtest::initTestCase(){
    coffeeSelection = new CoffeeSelection();

    insertCoinSensor = new CoinSensor();
    coinChecker = new CoinChecker(insertCoinSensor);

    pump = new Pump();
    milkPump = new Pump();
    waterPump = new Pump();

    touchScreen = new TouchScreen();

    waterValve = new Valve(valveType::water);
    steamValve = new Valve(valveType::steam);
    milkValve = new Valve(valveType::milk);
    freshWaterSupplyValve = new Valve(valveType::supply);

    valves = { waterValve, steamValve, milkValve, freshWaterSupplyValve };

    simulation = new Simulation(touchScreen, insertCoinSensor, pump, freshWaterSupplyValve);
    payment = new Payment(simulation, coffeeSelection);

    insertCoinSensor->setSimulation(simulation);

    coinSensors = {
        new CoinSensor(coinType::EUR2),
        new CoinSensor(coinType::EUR1),
        new CoinSensor(coinType::EUR05),
        new CoinSensor(coinType::EUR02),
        new CoinSensor(coinType::EUR01),
        new CoinSensor(coinType::EUR005)
    };

    coinLightSensors = {
        new LightSensor(simulation, lightSensors::hopper),
        new LightSensor(simulation, lightSensors::coinSupply)
    };

    maintenanceLightSensors = {
        new LightSensor(simulation, lightSensors::cremaBeanLevel),
        new LightSensor(simulation, lightSensors::espressoBeanLevel),
        new LightSensor(simulation, lightSensors::milkLevel),
        new LightSensor(simulation, lightSensors::dirtyWater),
        new LightSensor(simulation, lightSensors::wasteDisposale),
        new LightSensor(simulation, lightSensors::cupInserted),
        new LightSensor(simulation, lightSensors::freshWaterLevel)
    };

    for (auto* sensor : coinSensors)
        sensor->setSimulation(simulation);

    for (auto* coinLightSensor : coinLightSensors)
        simulation->setCoinLightSensorInSimulation(coinLightSensor->getSensorName(), coinLightSensor);

    coinSupply = new CoinSupply(simulation, coinSensors, coinLightSensors);

    payment->setCoinSupply(coinSupply);
    payment->setCoinChecker(coinChecker);
    coinChecker->setPayment(payment);

    coffeeStateMachine = new CoffeeStateMachine(simulation, payment, coinChecker, coffeeSelection, coinSupply);
    touchHandler = new TouchHandler(coffeeStateMachine, touchScreen);

    simulation->setTouchHandler(touchHandler);
    coffeeSelection->setTouchHandler(touchHandler);

    thermoblock = new Thermoblock(simulation);

    coffeeWaiter = new CoffeeWaiter(
        payment,
        maintenanceLightSensors[5], // cupInserted
        thermoblock,
        simulation,
        coffeeStateMachine,
        coffeeSelection
        );

    grinder = new Grinder(coffeeStateMachine, coffeeWaiter, simulation);
    coffeeWaiter->setGrinder(grinder);

    pumpControl = new PumpControl(simulation, pump);
    simulation->setPumpControl(pumpControl);

    brewingUnit = new BrewingUnit(coffeeStateMachine, simulation, coffeeWaiter, pumpControl, waterValve);
    milkUnit = new MilkUnit(
        thermoblock,
        coffeeStateMachine,
        simulation,
        brewingUnit,
        coffeeWaiter,
        pumpControl,
        milkPump,
        steamValve,
        milkValve
        );

    maintenance = new Maintenance(
        coffeeStateMachine,
        simulation,
        pumpControl,
        waterPump,
        waterValve,
        thermoblock,
        milkValve,
        maintenanceLightSensors,
        freshWaterSupplyValve,
        milkUnit
        );

    payment->setCoffeeStateMachine(coffeeStateMachine);
    coinChecker->setCoffeeStateMachine(coffeeStateMachine);
    coffeeStateMachine->setTouchHandler(touchHandler);
    maintenanceLightSensors[5]->setCoffeeStateMachine(coffeeStateMachine); // cupInserted

    lightSensors = {
        coinLightSensors[0],
        coinLightSensors[1],
        maintenanceLightSensors[0],
        maintenanceLightSensors[1],
        maintenanceLightSensors[2],
        maintenanceLightSensors[3],
        maintenanceLightSensors[4],
        maintenanceLightSensors[5],
        maintenanceLightSensors[6]
    };

    startUpManager = new StartUpManager(
        brewingUnit,
        coffeeSelection,
        coffeeStateMachine,
        coffeeWaiter,
        coinChecker,
        grinder,
        lightSensors,
        maintenance,
        milkUnit,
        payment,
        pump,
        pumpControl,
        simulation,
        thermoblock,
        touchHandler,
        touchScreen,
        valves
        );

    preparationShutdown = new PreparationShutdown(
        coinChecker,
        lightSensors,
        simulation,
        maintenance);


}

void systemtest::testEspressoMedium() {

    // Press Start Button
    simulation->onButtonTouched(buttons::start);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::readySelectCoffee);


    // Press Esperesso Button
    simulation->onButtonTouched(buttons::espresso);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::confirmIntensity);
    QCOMPARE(coffeeSelection->getTypeCoffee(), coffee::espresso);

    // Press Medium Intensity

    simulation->onButtonTouched(buttons::medium);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QCOMPARE(coffeeSelection->getCoffeeIntensity(), intensity::medium);
    QCOMPARE(coffeeSelection->getPriceInCent(), 200);
    QCOMPARE(payment->getCurrentAmountPaid(), 0);

    // Insert Coins

    simulation->onEurButtonClicked(coinType::EUR1);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QTRY_COMPARE(payment->getCurrentAmountPaid(), 100);

    simulation->onEurButtonClicked(coinType::EUR01);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QTRY_COMPARE(payment->getCurrentAmountPaid(), 110);

    simulation->onEurButtonClicked(coinType::EUR05);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QTRY_COMPARE(payment->getCurrentAmountPaid(), 160);


    simulation->onEurButtonClicked(coinType::EUR1);

    QTRY_COMPARE(simulation->getCoinReturn(coinType::EUR05), 1);
    QTRY_COMPARE(simulation->getCoinReturn(coinType::EUR01), 1);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::grindCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 0);


    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::heatingWater);
    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentTemperature(), 90, 15000);


    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCup);

    simulation->cupInserted(true);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::brewCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 16);

    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentPressure(), 9000, 10000);

    QTRY_COMPARE_WITH_TIMEOUT(coffeeStateMachine->getCurrentState(), state::takeOutCoffee, 15000);

    simulation->cupInserted(false);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::readySelectCoffee);

}


void systemtest::testCremaMedium() {


    // Press Crema Button
    simulation->onButtonTouched(buttons::crema);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::confirmIntensity);
    QCOMPARE(coffeeSelection->getTypeCoffee(), coffee::crema);

    // Press Medium Intensity

    simulation->onButtonTouched(buttons::medium);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QCOMPARE(coffeeSelection->getCoffeeIntensity(), intensity::medium);
    QCOMPARE(coffeeSelection->getPriceInCent(), 100);
    QCOMPARE(payment->getCurrentAmountPaid(), 0);

    // Insert Coins

    simulation->onEurButtonClicked(coinType::EUR1);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::grindCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 0);


    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::heatingWater);
    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentTemperature(), 90, 15000);

    simulation->cupInserted(true);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::brewCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 12);

    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentPressure(), 9000, 10000);

    QTRY_COMPARE_WITH_TIMEOUT(simulation->getWaterDispensed(), 150, 15000);

    QTRY_COMPARE_WITH_TIMEOUT(coffeeStateMachine->getCurrentState(), state::takeOutCoffee, 15000);

    simulation->cupInserted(false);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::readySelectCoffee);


}


void systemtest::testCapuccinoMedium() {


    // Press Capuccino Button
    simulation->onButtonTouched(buttons::cappuccino);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::confirmIntensity);
    QCOMPARE(coffeeSelection->getTypeCoffee(), coffee::cappuccino);

    // Press Medium Intensity

    simulation->onButtonTouched(buttons::medium);

    QCOMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QCOMPARE(coffeeSelection->getCoffeeIntensity(), intensity::medium);
    QCOMPARE(coffeeSelection->getPriceInCent(), 300);
    QCOMPARE(payment->getCurrentAmountPaid(), 0);

    // Insert Coins

    simulation->onEurButtonClicked(coinType::EUR1);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::waitingForCoin);
    QTRY_COMPARE(payment->getCurrentAmountPaid(), 100);

    simulation->onEurButtonClicked(coinType::EUR2);


    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::grindCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 0);


    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::heatingWater);
    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentTemperature(), 90, 15000);

    simulation->cupInserted(true);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::brewCoffee);
    QTRY_COMPARE(simulation->getBeansGrinded(), 16);

    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentPressure(), 9000, 10000);

    QTRY_COMPARE(simulation->getWaterDispensed(), 25);

    QTRY_COMPARE_WITH_TIMEOUT(simulation->getCurrentTemperature(), 120, 20000);

    QTRY_COMPARE_WITH_TIMEOUT(coffeeStateMachine->getCurrentState(), state::dispenseMilk, 15000);
    QTRY_COMPARE_WITH_TIMEOUT(simulation->getDispencedAmountMilk(), 60, 15000);

    QTRY_COMPARE_WITH_TIMEOUT(coffeeStateMachine->getCurrentState(), state::takeOutCoffee, 15000);

    simulation->cupInserted(false);

    QTRY_COMPARE(coffeeStateMachine->getCurrentState(), state::readySelectCoffee);


}


QTEST_MAIN(systemtest)

#include "tst_systemtest.moc"
