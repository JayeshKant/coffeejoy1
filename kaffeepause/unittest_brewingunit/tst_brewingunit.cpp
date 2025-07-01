#include "tst_brewingunit.h"
#include "qtest.h"
#include <cmath>



/* ───────────────────────── init / cleanup ───────────────────────── */

void TestBrewingUnit::init()
{
    sim      = new SimulationMock(this);
    pumpCtrl = new PumpControlMock(sim,this);
    valve    = new Valve(valveType::water,this);
    sm       = new StateMachineMock(this);
    info     = new InfoMock(coffee::espresso,this);
    waiter   = new WaiterMock(info,this);

    sut      = new BrewingUnit(sm,sim,waiter,pumpCtrl,valve,this);
}

void TestBrewingUnit::cleanup()
{
    delete sut;        sut      = nullptr;
    /* children (sim, pumpCtrl, …) deleted automatically by QObject tree */
}

/* ─────────────────────────── test cases ─────────────────────────── */

// Espresso should finish with coffeeReady signal
void TestBrewingUnit::testEspresso()
{
    QSignalSpy readySpy   (sut,     &BrewingUnit::coffeeReady);
    QSignalSpy finishedSpy(sut,     &BrewingUnit::brewingFinished);
    QSignalSpy startSpy   (pumpCtrl,&PumpControlMock::startPumpCalled);
    // QSignalSpy waterSpy   (sim,     &SimulationMock::pumpWaterCalled);

    // waiter->requestBrewing();          // emits startBrewing → sut::brewCoffee
    // pumpCtrl->startPumpForTest();      // simulate pump starting
    // emit pumpCtrl->targetPressureReached();
    // QVERIFY(valve->getValveState() == valveState::open);

    // // BrewingUnit should ask Simulation to pump the correct water amount
    // QVERIFY(waterSpy.count() == 1);
    // QList<QVariant> args = waterSpy.takeFirst();
    // QCOMPARE(args.at(0).toInt(), info->getAmountWater());
    // QCOMPARE(args.at(1).toInt(), 5);   // default flowRate

    emit sim->waterDispensed();        // pretend water done
    QVERIFY(valve->getValveState() == valveState::closed);

    // Espresso ⇒ coffeeReady exactly once
    QCOMPARE(readySpy.count(),    1);
    QCOMPARE(finishedSpy.count(), 0);
}

// Cappuccino should finish with brewingFinished signal
void TestBrewingUnit::testCappuccino()
{
    /* re-configure waiter/info for cappuccino */
    info->deleteLater();
    waiter->deleteLater();

    info   = new InfoMock(coffee::cappuccino,this);
    waiter = new WaiterMock(info,this);
    sut->deleteLater();
    sut    = new BrewingUnit(sm,sim,waiter,pumpCtrl,valve,this);

    QSignalSpy finishedSpy(sut,&BrewingUnit::brewingFinished);

    waiter->requestBrewing();
    pumpCtrl->startPumpForTest();
    emit pumpCtrl->targetPressureReached();
    emit sim->waterDispensed();

    QCOMPARE(finishedSpy.count(), 1);
}

/* ───────────────────── Qt-test main & moc include ────────────────── */

int main(int argc, char *argv[]) {
    ::QTest ::Internal ::callInitMain<TestBrewingUnit>();
    TestBrewingUnit tc;
    QTest ::setMainSourcePath(
        "C:\\Users\\Jayes\\kaffeepause\\kaffeepause\\unittest_brewingunit\\tst_"
        "brewingunit.cpp",
        "C:/Users/Jayes/kaffeepause/kaffeepause/main_project/build/"
        "unittest_brewingunit");
    return QTest ::qExec(&tc, argc, argv);
}
