#include <QTest>
#include "../app/include/simulation.h"
#include "../app/include/touchscreen.h"
#include "../app/include/coinsensor.h"
#include "../app/include/pump.h"
#include "../app/include/valve.h"
#include "../app/include/thermoblock.h"
#include "qsignalspy.h"

class ThermoblockTest : public QObject
{
    Q_OBJECT
private:
    TouchScreen *screen{nullptr};
    CoinSensor  *sensor{nullptr};
    Pump        *pump{nullptr};
    Valve       *waterValve{nullptr};
    Simulation  *sim{nullptr};
    Thermoblock *thermoblock{nullptr};

private slots:
    void init();         // runs before each test
    void cleanup();      // runs after each test

    void setCurrentMode_setsTargetTemperature();
    void isAtTargetTemperature_returnsExpected();
    void controlHeating_increasesTemperature();
    void controlHeating_stopsWhenLowWater();
    void controlHeating_emitsWaterOk();
    void controlHeating_emitsSteamOk();
};

// create real objects for every test
void ThermoblockTest::init()
{
    screen     = new TouchScreen;
    sensor     = new CoinSensor;
    pump       = new Pump;
    waterValve = new Valve(valveType::water);
    sim        = new Simulation(screen, sensor, pump, waterValve);
    thermoblock = new Thermoblock(sim);
}

void ThermoblockTest::cleanup()
{
    delete thermoblock;
    delete sim;
    delete waterValve;
    delete pump;
    delete sensor;
    delete screen;
}

void ThermoblockTest::setCurrentMode_setsTargetTemperature()
{
    thermoblock->setCurrentMode(mode::brewing);
    QCOMPARE(thermoblock->getTargetTemperature(), 90);

    thermoblock->setCurrentMode(mode::steaming);
    QCOMPARE(thermoblock->getTargetTemperature(), 120);

    thermoblock->setCurrentMode(mode::maintenance);
    QCOMPARE(thermoblock->getTargetTemperature(), 100);

    thermoblock->setCurrentMode(mode::standby);
    QCOMPARE(thermoblock->getTargetTemperature(), 60);
}

void ThermoblockTest::isAtTargetTemperature_returnsExpected()
{
    thermoblock->setCurrentMode(mode::brewing);
    sim->setCurrentTemperature(80);
    QVERIFY(!thermoblock->isAtTargetTemperature());

    sim->setCurrentTemperature(90);
    QVERIFY(thermoblock->isAtTargetTemperature());
}

void ThermoblockTest::controlHeating_increasesTemperature()
{
    thermoblock->setCurrentMode(mode::brewing);
    sim->setCurrentTemperature(80);

    QSignalSpy spy(thermoblock, &Thermoblock::temperatureChanged);
    thermoblock->controlHeating();

    QCOMPARE(sim->getCurrentTemperature(), 85);  // +5 °C step
    QCOMPARE(spy.count(), 1);
}

void ThermoblockTest::controlHeating_stopsWhenLowWater()
{
    thermoblock->setCurrentMode(mode::brewing);
    sim->setCurrentTemperature(80);

    // reduce to about 50 ml
    sim->pumpWater(450, 450);
    sim->reduceWater();

    thermoblock->controlHeating();
    QCOMPARE(sim->getCurrentTemperature(), 80);   // unchanged
}

void ThermoblockTest::controlHeating_emitsWaterOk()
{
    thermoblock->setCurrentMode(mode::brewing);
    sim->setCurrentTemperature(90);               // already at target

    QSignalSpy spy(thermoblock, &Thermoblock::waterTemperatureOK);
    thermoblock->controlHeating();

    QCOMPARE(thermoblock->getCurrentMode(), mode::standby);
    QCOMPARE(spy.count(), 1);
}

void ThermoblockTest::controlHeating_emitsSteamOk()
{
    thermoblock->setCurrentMode(mode::steaming);
    sim->setCurrentTemperature(120);

    QSignalSpy spy(thermoblock, &Thermoblock::steamingOK);
    thermoblock->controlHeating();

    QCOMPARE(thermoblock->getCurrentMode(), mode::standby);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(ThermoblockTest)
#include "tst_thermoblock.moc"
