 #include "coinchecker.h"
#include "coffeestatemachine.h"


CoinChecker::CoinChecker(CoinSensor* m_coinSensor, QObject* parent )  : QObject(parent), m_coinSensor(m_coinSensor) {
    qDebug() << "CoinChecker: created with sensor at" << m_coinSensor;

    pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &CoinChecker::checkInserted);
}

CoinChecker::~CoinChecker() {}

void CoinChecker::setPayment(Payment* m_payment){
    this->m_payment = m_payment;
    // connect(m_payment, &Payment::waitingForCoin, this, &CoinChecker::startTimer);
}

void CoinChecker::setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine) {
    this->m_coffeeStateMachine = m_coffeeStateMachine;
}

void CoinChecker::start(){
    pollTimer->start(500); // alle 200 ms prüfen
    qDebug() << "CoinChecker: polling timer started (200ms)";
}

void CoinChecker::shutdown(){
    qDebug("CoinChecker::stop");
     pollTimer->stop();
}

CoinSensor* CoinChecker::getCoinSensor(){
    return this->m_coinSensor;
}

void CoinChecker::checkCoin() {
    m_coinSensor->setCoinTypeFromCoinData(m_payment->getInsertedCoinData());

    if (m_coinSensor->getCoinIsValid()) {
        qDebug() << "CoinChecker: coin is VALID, emitting coinValid";
        m_coffeeStateMachine->trigger(event::COIN_VALID);
        emit coinValid();

    } else {
        qDebug() << "CoinChecker: coin is INVALID, emitting coinInvalid";
        m_coffeeStateMachine->trigger(event::COIN_INVALID);
        //TODO trigger Invalid und auskommentieren connect in statetmachine
        emit coinInvalid();
    }
}

void CoinChecker::checkInserted() {
    if(m_payment->getCoinIsInserted()){
        qDebug() << "CoinChecker: coin insertion detected, processing…";
        m_coinSensor->resetValues();
        m_payment->setCoinIsInserted(false); //reset for next coin
        emit coinDetected();
        checkCoin();
    }
}


