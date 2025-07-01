#include "coinsensor.h"
#include "simulation.h"

CoinSensor::CoinSensor(QObject* parent)
    : QObject(parent) {
    resetValues();
}

CoinSensor::CoinSensor(coinType coinType, QObject* parent)
    :QObject(parent){
    resetValues();
}

void CoinSensor::setSimulation(Simulation* m_simulation){
    this->m_simulation = m_simulation;
}

CoinSensor::~CoinSensor() {}

void CoinSensor::resetValues(){
    this->coinIsValid = false;
    this->detectedCoinType = coinType::RESET;
}

void CoinSensor::setCoinTypeFromCoinData(CoinData insertedCoinData) {

    for(const auto& [Type, coinData] : knownCoins()) {
        bool isKnownCoin = true;

        for (int i = 0; i < coinData.numberOfProperties; ++i) {
            if (coinData.at(i) != insertedCoinData.at(i)) {
                isKnownCoin = false;
                break;
            }
        }

        if (isKnownCoin) {
            setCoinIsValid(true);
            detectedCoinType = Type;
            qDebug() << "CoinSensor::setCoinTypeFromCoinData: Coin detected: " << (int)detectedCoinType;
            return;
        }
    }

    setCoinIsValid(false);
    detectedCoinType = coinType::INVALID;
    qDebug() << "CoinSensor::setCoinTypeFromCoinData: Invalid coin detected!";
}

coinType CoinSensor::getCoinType() {
    return this->detectedCoinType;
}

void CoinSensor::setCoinIsValid(bool coinIsValid){
    this->coinIsValid = coinIsValid;
}

bool CoinSensor::getCoinIsValid() {
    return this->coinIsValid;
}

unsigned int CoinSensor::getCoinAmounts(enum coinType coinType) {
    int availableRessourcesInMicrometer = getAvailableRessourcesInMicrometer(coinType);
    return((unsigned int)(availableRessourcesInMicrometer/ knownCoins().at(coinType).widthMicrometer));
}

int CoinSensor::getAvailableRessourcesInMicrometer(enum coinType coinType) {
    return(m_simulation->getCoinLevel(coinType));
}


std::map<coinType, CoinData> CoinSensor::knownCoins() {
    return {
        {coinType::EUR005, {2125, 167, 392, 0}},
        {coinType::EUR01, {1975, 193, 410, 1}},
        {coinType::EUR02, {2225, 214, 574, 2}},
        {coinType::EUR05, {2425, 238, 780, 3}},
        {coinType::EUR1, {2325, 167, 750, 4}},
        {coinType::EUR2, {2575, 167, 850, 5}}
    };
}


