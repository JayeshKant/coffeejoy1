#pragma once

#include <QObject>
#include <QtTest>

#include "pump.h"

// NOTE:
// The purpose of this class is to test the Pump class functionality.
// The Pump class is simple and has no dependencies, so no mock objects are needed.
class TestPump : public QObject
{
    Q_OBJECT

    Pump * sut;

public:
    explicit TestPump(QObject *parent = nullptr) : QObject(parent) {}

private slots:

    // Executed by QtTest before and after test suite
    void initTestCase() {}
    void cleanupTestCase() {}

    // Executed by QtTest before and after each test
    void init();
    void cleanup();

    // tests
    void testInitialState();
};
