#include <Qtest>
#include "../app/include/pump.h"

class PumpTest : public QObject
{
    Q_OBJECT
private slots:
    void defaultState();
    void setRunningTrue();
    void setRunningFalse();
    void resetStopsRunning();
};

void PumpTest::defaultState()
{
    Pump p;
    QCOMPARE(p.isRunning(), false);
}

void PumpTest::setRunningTrue()
{
    Pump p;
    p.setRunning(true);
    QVERIFY(p.isRunning());
}

void PumpTest::setRunningFalse()
{
    Pump p;
    p.setRunning(true);
    p.setRunning(false);
    QVERIFY(!p.isRunning());
}

void PumpTest::resetStopsRunning()
{
    Pump p;
    p.setRunning(true);
    p.reset();
    QVERIFY(!p.isRunning());
}

QTEST_APPLESS_MAIN(PumpTest)
#include "tst_pump.moc"
