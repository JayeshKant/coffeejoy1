#include <QTest>

// add necessary includes here

class coffeewaiter : public QObject
{
    Q_OBJECT

public:
    coffeewaiter();
    ~coffeewaiter();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
};

coffeewaiter::coffeewaiter() {}

coffeewaiter::~coffeewaiter() {}

void coffeewaiter::initTestCase() {}

void coffeewaiter::cleanupTestCase() {}

void coffeewaiter::test_case1() {}

QTEST_APPLESS_MAIN(coffeewaiter)

#include "tst_coffeewaiter.moc"
