#pragma once

#include <QObject>
#include <QtTest>

#include "coinsensor.h"
#include "payment.h"

// NOTE:
// The purpose of this class is to test the CoinSensor functionality.
// This test focuses on the resetValues() method functionality.
class TestCoinSensor : public QObject
{
    Q_OBJECT

    CoinSensor * sut;

public:
    explicit TestCoinSensor(QObject *parent = nullptr)  : QObject(parent) {}

private slots:

    // Executed by QtTest before and after test suite
    void initTestCase() {}
    void cleanupTestCase() {}

    // Executed by QtTest before and after each test
    void init();
    void cleanup();

    // tests
    void testResetValues();
};
