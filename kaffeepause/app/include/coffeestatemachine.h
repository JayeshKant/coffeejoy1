#pragma once

#include <QObject>
#include <QString>

// #include "coffeetypes.h"
// #include "coffeewaiter.h"
#include "coinchecker.h"
#include "payment.h"
#include "coinsupply.h"
#include "simulation.h"

enum class state {
    preparationStart     = 1,
    readySelectCoffee    = 2,
    confirmIntensity     = 3,

    payment              = 4,
    waitingForCoin       = 41,
    coinInserted         = 42,
    checkCoin            = 43,
    validCoin            = 44,
    bookCoin             = 45,
    returnInvalid        = 46,
    returnChange         = 47,

    makeCoffee           = 5,
    grindCoffee          = 51,
    heatingWater         = 52,
    waitingForCup        = 53,
    brewCoffee           = 54,
    dispenseMilk         = 6,
    takeOutCoffee        = 7,

    maintenance          = 8,
    abort                = 9,
    performShutdown      = 10

}; // TODO add cases in windowui at updatesButtons

enum class event {
    START_UP,
    START_UP_DONE,
    COFFEE_SELECTED,
    INTENSITY_CONFIRMED,

    COIN_INSERTED,
    COIN_DETECTED,
    COIN_VALID,
    COIN_INVALID,
    COIN_INVALID_RETURNED,
    COIN_STUCK,
    COIN_BOOKED,
    NEEDED_AMOUNT_REACHED,
    NEEDED_AMOUNT_NOT_REACHED,
    PAYMENT_DONE,

    PREPARE_COFFEE,
    GRINDING_COMPLETET,
    WATER_TEMPERATURE_OK,
    CUP_INSERTED,
    BREWING_COMPLETET,
    DISPENSED_COFFEE,

    COFFEE_READY_TO_TAKEOUT,
    MAINTENANCE_REQUESTED,
    DECALCIFICATION_REQUESTED,
    SHUTDOWN_REQUESTED,
    ABORT_REQUESTED,
    ERROR,
    FINISHED
};

class TouchHandler;

class CoffeeStateMachine : public QObject {
    Q_OBJECT

    state current_state_ = state::preparationStart;

public:
    CoffeeStateMachine(
        Simulation* m_simulation,
        Payment* m_payment,
        CoinChecker* m_coinChecker,
        CoffeeSelection* m_coffeeSelection,
        CoinSupply* m_coinSupply,
        QObject* parent = nullptr);
    ~CoffeeStateMachine();


    void trigger(enum event event);

    virtual state getCurrentState();
    int getCurrentAmountPaid();
    void setTouchHandler(TouchHandler* m_touchHandler);

signals:
    void readySelectCoffee();
    void newBalance();
    void waitForCup();
    void steamMilk();

    void coffeeReadyToDispense();
    void coffeeReadyToTakout();
    void shutdownRequested();
    void maintenanceRequested();
    void decalcificationRequested();
    void abortRequested();

    void stateChanged(state newState);

private:
    Simulation* m_simulation;

    Payment* m_payment;

    CoinChecker* m_coinChecker;

    CoffeeSelection* m_coffeeSelection;

    TouchHandler* m_touchHandler;

    CoinSupply* m_coinSupply;

    state currentState;


};
