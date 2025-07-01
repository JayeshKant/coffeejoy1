#pragma once

#include <QObject>
#include <QtTest>

#include "brewingunit.h"
#include "pump.h"
#include "pumpcontrol.h"
#include "simulation.h"
#include "valve.h"
#include "coffeeinformation.h"
#include "coffeewaiter.h"
#include "coffeestatemachine.h"

/* ───────────────────────── Mocks / Fakes ───────────────────────── */

class PumpMock : public Pump
{
    Q_OBJECT
public:
    using Pump::Pump;                           // inherit trivial ctor
};

class SimulationMock : public Simulation
{
    Q_OBJECT
public:
    explicit SimulationMock(QObject *parent = nullptr)
        : Simulation(nullptr,nullptr,new PumpMock(this),
                     new Valve(valveType::supply,this), parent) {}

    /* BrewingUnit calls this – record the params & emit a spy signal */
    void pumpWater(int ml,int flow)
    {
        lastMl   = ml;
        lastFlow = flow;
        emit pumpWaterCalled(ml,flow);
    }

    int lastMl   {0};
    int lastFlow {0};

signals:
    void pumpWaterCalled(int ml,int flow);
};

class PumpControlMock : public PumpControl
{
    Q_OBJECT
public:
    explicit PumpControlMock(Simulation *sim,QObject *parent=nullptr)
        : PumpControl(sim,new PumpMock(this),parent) {}

    void startPumpForTest()            // wrapper so tests can spy
    {
        startPump();                   // ⇢ real implementation
        emit startPumpCalled();
    }
signals:
    void startPumpCalled();
};

class StateMachineMock : public CoffeeStateMachine
{
    Q_OBJECT
public:
    explicit StateMachineMock(QObject *parent=nullptr)
        : CoffeeStateMachine(nullptr,nullptr,nullptr,nullptr,nullptr,parent) {}

    void trigger(enum event e)              // shadow, just log
    { lastEvent=e; emit triggered(); }

    enum event lastEvent{event::ERROR};

signals:
    void triggered();
};

class InfoMock : public CoffeeInformation
{
public:
    InfoMock(coffee type=coffee::espresso,QObject *parent=nullptr)
        : CoffeeInformation(intensity::medium,type,parent) {}
};

class WaiterMock : public CoffeeWaiter
{
    Q_OBJECT
public:
    explicit WaiterMock(InfoMock *info,QObject *parent=nullptr)
        : CoffeeWaiter(nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,parent),
        info_(info) {}

    CoffeeInformation *getCoffeeInformation()  { return info_; }
    void requestBrewing() { emit startBrewing(); }
private:
    InfoMock *info_;
};

/* ─────────────────── Test class declaration only ────────────────── */

class TestBrewingUnit : public QObject
{
    Q_OBJECT

    /* SUT + collaborators */
    BrewingUnit      *sut      {nullptr};
    SimulationMock   *sim      {nullptr};
    PumpControlMock  *pumpCtrl {nullptr};
    Valve            *valve    {nullptr};
    StateMachineMock *sm       {nullptr};
    InfoMock         *info     {nullptr};
    WaiterMock       *waiter   {nullptr};

private slots:
    /* called once for whole suite */
    void initTestCase()   {}
    void cleanupTestCase(){}

    /* called before / after each testcase */
    void init();
    void cleanup();

    /* the actual test cases */
    void testEspresso();      // → coffeeReady
    void testCappuccino();    // → brewingFinished
};
