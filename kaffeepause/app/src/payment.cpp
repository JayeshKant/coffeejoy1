#include "payment.h"
#include "coinchecker.h"
#include "coffeestatemachine.h"
#include "coinsensor.h"
#include "simulation.h"

#include <QDebug>


Payment::Payment(Simulation* m_simulation, CoffeeSelection* m_coffeeSelection, QObject* parent )
    : QObject(parent), m_simulation(m_simulation), m_coffeeSelection(m_coffeeSelection){
    qDebug() << "Payment: initialized. Amount due =" << m_coffeeSelection->getPriceInCent() << "€";

    connect(this, &Payment::coinBooked, this, &Payment::onCoinBooked);
    connect(this, &Payment::requiredAmountReached, this, &Payment::onRequiredAmountReached);
    connect(m_simulation, &Simulation::coinInserted, this, &Payment::setInsertedCoinData);

    coinReturnMap = {
        {coinType::EUR005, 0},
        {coinType::EUR01, 0},
        {coinType::EUR02, 0},
        {coinType::EUR05, 0},
        {coinType::EUR1, 0},
        {coinType::EUR2, 0}
    };

}

Payment::~Payment() {}

void Payment::setCoinChecker(CoinChecker* m_coinChecker) { //ask what is it doing
    this->m_coinChecker = m_coinChecker;
    connect(m_coinChecker, &CoinChecker::coinValid, this, &Payment::setCurrentAmountPaid);
    connect(m_coinChecker, &CoinChecker::coinInvalid, this, &Payment::onCoinInvalid);
}

void Payment::setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine) {
    this->m_coffeeStateMachine = m_coffeeStateMachine;
}

void Payment::reset(){
    qDebug("Payment::reset");

    amountPaid = 0;
    amountChange = 0;

    coinReturnMap = {
        {coinType::EUR005, 0},
        {coinType::EUR01, 0},
        {coinType::EUR02, 0},
        {coinType::EUR05, 0},
        {coinType::EUR1, 0},
        {coinType::EUR2, 0}
    };

    coinIsInserted = false;
    coinIsValid = false;
}


bool Payment::getCoinIsInserted() {
    return this->coinIsInserted;
}

void Payment::setCoinIsInserted(bool coinIsInserted) {
    this->coinIsInserted = coinIsInserted;
}

void Payment::setCoinSupply(CoinSupply* coinSupply) {
    m_coinSupply = coinSupply;

    connect(m_coinSupply, &CoinSupply::coinStoredSuccessfully, this, &Payment::onStorageSuccess);

    connect(m_coinSupply, &CoinSupply::coinStorageFailed, this, &Payment::onStorageFailed);
}



void Payment::setInsertedCoinData(CoinData insertedCoinData) {
    this->insertedCoinData = insertedCoinData;
    this->setCoinIsInserted(true);
}

CoinData Payment::getInsertedCoinData(){
    return this->insertedCoinData;
}


int Payment::getCheckedInsertedCoinValue(){
    return this->amountPaid;
}



void Payment::setCurrentAmountPaid(){
    this->m_coinSensor = m_coinChecker->getCoinSensor();
    qDebug() << "Payment::setCurrentAmountPaid getCoinSensor";

    coinType detected_cointype = m_coinSensor->getCoinType();
    this->m_coinSupply->storeCoin(detected_cointype);

}


void Payment::onStorageSuccess(coinType detecedType){
    qDebug() << "Payment::setCurrentAmountPaid Coin stored:" << (int)detecedType;
    this->amountPaid += (int)detecedType;
    qDebug() << "Payment::setCurrentAmountPaid total Amount Paid:" << amountPaid;
    m_coffeeStateMachine->trigger(event::COIN_BOOKED);
    emit coinBooked();
}


void Payment::onStorageFailed(coinType detecedType){
    qWarning() << "Failed to store coin:" << (int)detecedType;
}


int Payment::getCurrentAmountPaid() {
    return this->amountPaid;
}


bool Payment::checkNeededAmountReached() {
    return (m_coffeeSelection->getPriceInCent() <= amountPaid);
}

void Payment::onCoinBooked(){
    if (checkNeededAmountReached()) {
        qDebug() << "onCoinBooked Called and about to emit neededAmountReached";
        m_coffeeStateMachine->trigger(event::NEEDED_AMOUNT_REACHED);
        emit requiredAmountReached();
    } else {
        qDebug() << "onCoinBooked Called and about to emit waitingForCoin.";
        m_coffeeStateMachine->trigger(event::NEEDED_AMOUNT_NOT_REACHED);
        emit waitingForCoin();
    }
}

void Payment::onRequiredAmountReached(){
    int amountChange;
    amountChange = this->getCurrentAmountPaid() - m_coffeeSelection->getPriceInCent();
    this->amountPaid = amountChange;
    qDebug() << "Payment::onRequiredAmountReached Your change:" << amountChange;

    if (amountChange > 0){
        // return money
        qDebug() << "Your change:" << amountChange;
        this->calculateChange(amountChange);
        m_coffeeStateMachine->trigger(event::PAYMENT_DONE);
        emit paymentDone();
    } else {
        qDebug() << "No Change";
        amountPaid = 0;
        emit newBalance();
        m_coffeeStateMachine->trigger(event::PAYMENT_DONE);
        emit paymentDone();
    }

}


void Payment::coinValue(int coin) {
    qDebug() << "Payment::coinvalue() called with coin=" << coin;
}


void Payment::manageInsertedObject(int unknownCoin[4] ){
    qDebug() << "Payment::manageInsertedObject(): raw detection data =" << unknownCoin[0] << unknownCoin[1]<< unknownCoin[2] << unknownCoin[3];

}

void Payment::calculateChange(int changeToBePaidOut){
    m_coinSupply->updatedCoinBalance();
    int tmpCalculatedChangePerCoinType = 0;
    int countReleasedCoinsPerCoinType = 0;
    std::vector<coinType> PossibleChangeCoins = {coinType::EUR2, coinType::EUR1, coinType::EUR05,
                                         coinType::EUR02, coinType::EUR01, coinType::EUR005};

    for (coinType type: PossibleChangeCoins){
        if(changeToBePaidOut >= 5){
            //Start calculation and releasing Coins
            tmpCalculatedChangePerCoinType = changeToBePaidOut/(int)type;
            countReleasedCoinsPerCoinType = controlledRelease(type, tmpCalculatedChangePerCoinType);
            changeToBePaidOut -= (int)type * countReleasedCoinsPerCoinType;
        }else if(changeToBePaidOut == 0){
            qDebug() << "Change has been fully returned.";
            return;
        }else if(changeToBePaidOut < 0){
            qFatal() << "Payment::calculateChange: You had one job only,"
                        " now we lost money! \n Money Lost" << changeToBePaidOut << " EUR" ;
        }else{
            qFatal() << "Payment::calculateChange: Can't pay out the remaining change: "
                     << changeToBePaidOut << " EUR";
        }
    }
}


int Payment::controlledRelease(coinType l_coinType, int counterCoins){
     int releasedCoins = 0;
    qDebug() << "Payment::controlledRelease(): trying to release: "<< counterCoins
              << " of coins of type" << (int)l_coinType;
    for(int i = counterCoins; i > 0; i--){
        if(m_coinSupply->releaseCoin(l_coinType)){
            amountPaid -= (int)l_coinType;
            m_simulation->setCoinReturn(l_coinType);
            emit newBalance();
            releasedCoins += 1;
            counterCoins -= 1;
        }else{
            break;
        }
    }

    //TODO return int amount of coins released
    qDebug() << "Payment::controlledRelease: released: " << releasedCoins
             << " Coins of Type "<< (int)l_coinType << "The following amount of "
                "coins of this type need to be paid out in different coins: " << counterCoins;
    return releasedCoins;
}

void Payment::onCoinInvalid(){
    qDebug() << "Payment::onCoinInvalid return invalid coin";
    m_simulation->addToCoinReturnMap(coinType::INVALID);
    // emit returnedInvalid();
    m_coffeeStateMachine->trigger(event::COIN_INVALID_RETURNED);
}

