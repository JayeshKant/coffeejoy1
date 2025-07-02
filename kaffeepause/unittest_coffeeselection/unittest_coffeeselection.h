#pragma once

#include <QObject>
#include <QtTest>

#include "coffeeselection.h"
#include "coffeetypes.h"

// NOTE:
// The purpose of this class is to test the CoffeeSelection class functionality.
// This test focuses on getter/setter methods for coffee types and intensities.
class TestCoffeeSelection : public QObject
{
    Q_OBJECT

    CoffeeSelection * sut;

public:
    explicit TestCoffeeSelection(QObject *parent = nullptr)  : QObject(parent) {}

private slots:

    // Executed by QtTest before and after test suite
    void initTestCase() {}
    void cleanupTestCase() {}

    // Executed by QtTest before and after each test
    void init();
    void cleanup();

    // tests
    void testGetSetTypeCoffee();
};
