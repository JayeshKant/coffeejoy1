#pragma once

#include <QObject>
#include <QtTest>

#include "coinchecker.h"
#include "coinsensor.h"
#include "payment.h"
#include "coffeestatemachine.h"

// Simple mock for CoinSensor based on existing unittest patterns
class CoinSensorMock : public CoinSensor
{
public:
    CoinSensorMock() : CoinSensor() {}
    virtual ~CoinSensorMock() = default;
};

// NOTE:
// The purpose of this class is to test the CoinChecker basic functionality.
// This test focuses on constructor, destructor and basic getter methods.
class TestCoinChecker : public QObject
{
    Q_OBJECT

    CoinChecker * sut;
    CoinSensorMock * mockCoinSensor;

public:
    explicit TestCoinChecker(QObject *parent = nullptr) : QObject(parent) {}

private slots:

    // Executed by QtTest before and after test suite
    void initTestCase() {}
    void cleanupTestCase() {}

    // Executed by QtTest before and after each test
    void init();
    void cleanup();

    // tests
    void testConstructorAndGetter();
};
