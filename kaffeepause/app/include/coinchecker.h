#pragma once

#include <QObject>
#include <QTimer>
#include <QDebug>

#include "coinsensor.h"
#include "payment.h"

class CoffeeStateMachine;

class CoinChecker : public QObject
{
    Q_OBJECT

public:
    CoinChecker(CoinSensor* m_coinSensor, QObject* parent = nullptr);
    ~CoinChecker();

    void start();

    void shutdown();

    void setPayment(Payment* m_payment);
    void setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine);

    CoinSensor* getCoinSensor();


signals:
    void coinDetected();
    void coinValid(); //coinChecker
    void coinInvalid(); //coinChecker

private:

    Payment* m_payment;
    QTimer* pollTimer;
    CoinSensor* m_coinSensor;
    CoffeeStateMachine* m_coffeeStateMachine;

    void checkCoin();


protected:
    void checkInserted();


};

