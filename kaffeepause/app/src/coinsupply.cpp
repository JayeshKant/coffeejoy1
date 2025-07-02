#include "coinsupply.h"
#include "qdebug.h"
#include "simulation.h"

using std::array;

CoinSupply::CoinSupply(Simulation* m_simulation, array<CoinSensor*, 6> m_coinSensors, array<LightSensor*, 2> m_coinLightSensors, QObject* parent)
    : QObject(parent), m_simulation(m_simulation) {



    if (!idealCoinBalance.empty()){
        idealCoinBalance.clear();
    }
    idealCoinBalance.insert(pair(coinType::EUR2, (unsigned int)50));
    idealCoinBalance.insert(pair(coinType::EUR1, (unsigned int)50));
    idealCoinBalance.insert(pair(coinType::EUR05,(unsigned int)50));
    idealCoinBalance.insert(pair(coinType::EUR02,(unsigned int)50));
    idealCoinBalance.insert(pair(coinType::EUR01, (unsigned int)50));
    idealCoinBalance.insert(pair(coinType::EUR005, (unsigned int)50));

    sensEUR2 = m_coinSensors.at(0);
    sensEUR1 = m_coinSensors.at(1);
    sensEUR05 = m_coinSensors.at(2);
    sensEUR02 = m_coinSensors.at(3);
    sensEUR01 = m_coinSensors.at(4);
    sensEUR005 = m_coinSensors.at(5);
    sensHopper = m_coinLightSensors.at(0);
    sensCoinSupply = m_coinLightSensors.at(1);

    connect(sensHopper, &LightSensor::timeoutError, this, [this]() {
        try {
            throw std::runtime_error("Timer timeout, Coinsupply no Object detected");
    } catch (const std::exception& e) {
            qFatal() << "Fatal Error: coin stuck in CoinSupply" << e.what();
        emit coinStuck("Fatal: coin stuck");
    }
    } ) ;

    connect(sensHopper, &LightSensor::coinDetectionSuccess, this, &CoinSupply::onCoinDetectionSucccess);

    connect(sensCoinSupply, &LightSensor::coinDetectionSuccess, this, &CoinSupply::onCoinDetectionSucccess);

    // if (!availableCoinBalance.empty()){
    //     availableCoinBalance.clear();
    // }

    qDebug() << "Initialize Coinsupply: sensEur2GetCoinAmounts amounts: " << sensEUR2->getCoinAmounts(coinType::EUR2);

    availableCoinBalance.insert(pair(coinType::EUR2, sensEUR2->getCoinAmounts(coinType::EUR2)));
    availableCoinBalance.insert(pair(coinType::EUR1, sensEUR1->getCoinAmounts(coinType::EUR1)));
    availableCoinBalance.insert(pair(coinType::EUR05, sensEUR05->getCoinAmounts(coinType::EUR05)));
    availableCoinBalance.insert(pair(coinType::EUR02, sensEUR02->getCoinAmounts(coinType::EUR02)));
    availableCoinBalance.insert(pair(coinType::EUR01, sensEUR01->getCoinAmounts(coinType::EUR01)));
    availableCoinBalance.insert(pair(coinType::EUR005, sensEUR005->getCoinAmounts(coinType::EUR005)));

    hopperCoinBalance.insert(pair(coinType::EUR2, (unsigned int)0));
    hopperCoinBalance.insert(pair(coinType::EUR1, (unsigned int)0));
    hopperCoinBalance.insert(pair(coinType::EUR05, (unsigned int)0));
    hopperCoinBalance.insert(pair(coinType::EUR02, (unsigned int)0));
    hopperCoinBalance.insert(pair(coinType::EUR01, (unsigned int)0));
    hopperCoinBalance.insert(pair(coinType::EUR005, (unsigned int)0));

    m_knownCoins = sensEUR2->knownCoins();

    qDebug() << "SensEur2 CoinSupply Konstruktor: " << sensEUR2;


    if(this->getRemainingCoinSupply() < 190){
        qDebug() << "CoinSupply::CoinSupply Maybe not enough change!";
        m_simulation->setEnoughChange(false);
    }

}




CoinSupply::~CoinSupply() {
    delete sensEUR2;
    delete sensEUR1;
    delete sensEUR05;
    delete sensEUR02;
    delete sensEUR01;
    delete sensEUR005;

}

void CoinSupply::countAvailableCoins(){
    qDebug() << "SensEur2 CoinSupply Konstruktor: " << this->sensEUR2;
    qDebug() << "CoinSupply::countAvailableCoins: sensEUR2 getCoinAmounts: " << sensEUR2->getCoinAmounts(coinType::EUR2);

    // TODO remove debugs when cleaning maybe

    this->availableCoinBalance.at(coinType::EUR2)   = sensEUR2->getCoinAmounts(coinType::EUR2);
    availableCoinBalance.at(coinType::EUR1)   = sensEUR1->getCoinAmounts(coinType::EUR1);
    availableCoinBalance.at(coinType::EUR05)  = sensEUR05->getCoinAmounts(coinType::EUR05);
    availableCoinBalance.at(coinType::EUR02)  = sensEUR02->getCoinAmounts(coinType::EUR02);
    availableCoinBalance.at(coinType::EUR01)  = sensEUR01->getCoinAmounts(coinType::EUR01);
    availableCoinBalance.at(coinType::EUR005) = sensEUR005->getCoinAmounts(coinType::EUR005);
}

void CoinSupply::checkCoinSupply(){

    for (auto const& [key, val] : availableCoinBalance)
        switch(key){
            case coinType::EUR2:
                availableCoinBalance[key] = sensEUR2->getCoinAmounts(key);
                break;
            case coinType::EUR1:
                availableCoinBalance[key] = sensEUR1->getCoinAmounts(key);
                break;
            case coinType::EUR05:
                availableCoinBalance[key] = sensEUR05->getCoinAmounts(key);
                break;
            case coinType::EUR02:
                availableCoinBalance[key] = sensEUR02->getCoinAmounts(key);
                break;
            case coinType::EUR01:
                availableCoinBalance[key] = sensEUR01->getCoinAmounts(key);
                break;
            case coinType::EUR005:
                availableCoinBalance[key] = sensEUR005->getCoinAmounts(key);
                break;
            default:
                qDebug() << "Invalid Coin, Error: This should have been handled by coinsensor"; // this case shouldn't happen, already handled in coinsensor



        }
}



bool CoinSupply::releaseCoin(coinType l_type){

    int coinWidth = sensEUR2->knownCoins().at(l_type).widthMicrometer;
    int availableRessources = 0;

    switch (l_type) {
        case coinType::EUR2:

            availableRessources = sensEUR2->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            };
            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;


        case coinType::EUR1:

            availableRessources = sensEUR1->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            };
            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;

        case coinType::EUR05:
            availableRessources = sensEUR05->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            };
            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;

        case coinType::EUR02:

            availableRessources = sensEUR02->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            };
            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;

        case coinType::EUR01:
            availableRessources = sensEUR01->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            };
            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;

        case coinType::EUR005:

            availableRessources = sensEUR005->getAvailableRessourcesInMicrometer(l_type);
            if(availableRessources >= m_knownCoins.at(l_type).widthMicrometer){
                m_simulation->setCoinLevel(l_type,(availableRessources-coinWidth));
                m_simulation->addToCoinReturnMap(l_type);
                break;
            }else if(availableRessources == 0){
                qDebug() << "CoinSupply::releaseCoin couldn't release the remaining Change, Sorry!";
                if (m_simulation->getEnoughChange() == false) {
                    return true;
                }else if (m_simulation->getEnoughChange() == true) {
                    qFatal() << "Did not warn the customer and was not able to return the change,"
                              "please talk to the Cashier.";
                }
            };

            qDebug() << "CoinSupply::releaseCoin no coins to release of type: "
                     << (int)l_type;
            return false;

        default:
            qDebug() << "CoinSupply::releaseCoin Invalid CoinType for Change! ERROR";
            return false;
        };
    this->updatedCoinBalance();
        return true;
}

void CoinSupply::closeAllInternalCoinSlots()
{
    //removes voltage from flap/ checks the status and then toggles it
    m_simulation->closeAllCoinSupplyBlocker();
}

void CoinSupply::openRelevantCoinSlots(coinDestination l_coinDestination)
{
    if(m_simulation->getStatusOfBlocker(l_coinDestination) == statusOfBlocker::closed){
        m_simulation->setCoinFlap(l_coinDestination);
    }
}


bool CoinSupply::moveCoin(coinDestination destination) {
    //TODO this shouldn't be a bool

    closeAllInternalCoinSlots();
    openRelevantCoinSlots(destination);

    detection startStatusPolling;
    startStatusPolling = sensCoinSupply->detectLight();
    qDebug() << "CoinSupply::moveCoin startStatusPolling: " << startStatusPolling;

    switch (destination) {
    case coinDestination::cashRegister:
        qDebug()<< "moveCoin to coinDestination::cashRegister";
        sensCoinSupply->detectedCoinObject(startStatusPolling);
        break;
    case coinDestination::hopper:
        qDebug()<< "moveCoin to coinDestination::hopper";
        sensHopper->detectedCoinObject(startStatusPolling);
        break;
    default:
        qDebug()<< "moveCoin reached an unsuported case";
        break;
    }
    return true;

}



void CoinSupply::storeCoin(coinType l_coinType){
    countAvailableCoins();

    qDebug() << "CoinSupply::storeCoin availableCoinBalance " << &this->availableCoinBalance ;
    qDebug() << "CoinSupply::storeCoin idealCoinBalance " << &this->idealCoinBalance;

    if(this->getRemainingCoinSupply() < 190){
        qDebug() << "CoinSupply::storeCoin Maybe not enough change!";
        m_simulation->setEnoughChange(false);
    }


    QTimer* pollDetectionSignal = new QTimer(this);
    connect(pollDetectionSignal, &QTimer::timeout, this, [this, l_coinType, pollDetectionSignal] (){
        if(coinDetectionSuccess){
            switch (l_coinDestination) {
            case coinDestination::cashRegister:
                m_simulation->addCoin(l_coinType,m_knownCoins.at(l_coinType).widthMicrometer);
                countAvailableCoins();
                pollDetectionSignal->stop();
                coinDetectionSuccess = false;
                qDebug() << "CoinSupply::storeCoin->pollDetectionSignal added cointype: " << (int)l_coinType << "to cashRegister, coinbalance of type: " << availableCoinBalance[l_coinType];
                emit coinStoredSuccessfully(l_coinType);
                break;
            case coinDestination::hopper:
                hopperCoinBalance[l_coinType] += 1;
                pollDetectionSignal->stop();
                coinDetectionSuccess = false;
                qDebug() << "CoinSupply::storeCoin->pollDetectionSignal added cointype: " << (int)l_coinType << "to hopper, coinbalance of type: " << hopperCoinBalance[l_coinType];
                emit coinStoredSuccessfully(l_coinType);
                break;
            default:
                qDebug() << "CoinSupply::storeCoin->pollDetectionSignal default case, you missed something!";
                pollDetectionSignal->stop();
                coinDetectionSuccess = false;
                emit coinStorageFailed(l_coinType);
                break;
            }

        }
    });


    if (this->idealCoinBalance[l_coinType] > this->availableCoinBalance[l_coinType] ){
        l_coinDestination = coinDestination::cashRegister;
        pollDetectionSignal->start(100);
        moveCoin(l_coinDestination);
    } else if (idealCoinBalance[l_coinType] <= availableCoinBalance[l_coinType] ){
        l_coinDestination = coinDestination::hopper;
        pollDetectionSignal->start(100);
        moveCoin(l_coinDestination);
    }else {
        return;
        //TODO exception
    }
}

int CoinSupply::getRemainingCoinSupply(){
    this->updatedCoinBalance();
    int remainingChange = 0;
    remainingChange = availableCoinBalance.at(coinType::EUR005)* (int)coinType::EUR005;
    remainingChange += availableCoinBalance.at(coinType::EUR01)* (int)coinType::EUR01;
    remainingChange += availableCoinBalance.at(coinType::EUR02)* (int)coinType::EUR02;
    remainingChange += availableCoinBalance.at(coinType::EUR05)* (int)coinType::EUR05;
    remainingChange += availableCoinBalance.at(coinType::EUR1)* (int)coinType::EUR1;

    return remainingChange;
}

std::map<coinType, unsigned int> CoinSupply::updatedCoinBalance(){
    checkCoinSupply();
    return availableCoinBalance;
}


void CoinSupply::onCoinDetectionSucccess(bool success)
{
    coinDetectionSuccess = success;
}


//int CoinSupply::releaseCoin(coinType){}
