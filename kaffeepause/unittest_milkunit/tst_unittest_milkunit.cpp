#include <QtTest>

// add necessary includes here

class unittest_milkUnit : public QObject
{
    Q_OBJECT

public:
    unittest_milkUnit();
    ~unittest_milkUnit();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

unittest_milkUnit::unittest_milkUnit() {}

unittest_milkUnit::~unittest_milkUnit() {}

void unittest_milkUnit::initTestCase() {}

void unittest_milkUnit::cleanupTestCase() {}

void unittest_milkUnit::test_case1() {}

QTEST_APPLESS_MAIN(unittest_milkUnit)

#include "tst_unittest_milkunit.moc"
