#include <QTest>
#include "valve.h"

class ValveTest : public QObject
{
    Q_OBJECT
private slots:
    void initialStateClosed();
    void setOpen();
    void setClosed();
};

void ValveTest::initialStateClosed()
{
    Valve v(valveType::water);
    QCOMPARE(v.getValveState(), valveState::closed);
}

void ValveTest::setOpen()
{
    Valve v(valveType::steam);
    v.setValveState(valveState::open);
    QCOMPARE(v.getValveState(), valveState::open);
}

void ValveTest::setClosed()
{
    Valve v(valveType::milk);
    v.setValveState(valveState::open);
    v.setValveState(valveState::closed);
    QCOMPARE(v.getValveState(), valveState::closed);
}

QTEST_APPLESS_MAIN(ValveTest)
#include "tst_valve.moc"
