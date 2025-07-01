#pragma once

#include <QObject>
#include "payment.h"
#include <map>

class Simulation;

class CoinSensor : public QObject{
    Q_OBJECT
public:
    CoinSensor(QObject* parent = nullptr);
    CoinSensor(coinType coinType, QObject* parent = nullptr);
    ~CoinSensor();

    void setSimulation(Simulation* m_simulation);

    void resetValues();

    void setCoinTypeFromCoinData(CoinData insertedCoinData);
    coinType getCoinType();

    void setCoinIsValid(bool coinIsValid);
    bool getCoinIsValid();

    unsigned int getCoinAmounts(coinType);

    std::map<coinType, CoinData> knownCoins();
    int getAvailableRessourcesInMicrometer(coinType);

private:
    Simulation* m_simulation;
    bool coinIsValid;
    coinType detectedCoinType;
};


/*
class CoinSensor
{
public:
    CoinSensor(int thickness); //without weight for now
    ~CoinSensor();
    int getAvaialableRessourcesInMicrometer();
    void setAvailableRessourcesInMicrometer();
    int getCoinAmounts();
    void setCoinAmounts();
    bool coinReceived(int); //time in ms seconds to evaluate

private:
    int mThickness;

};
*/
