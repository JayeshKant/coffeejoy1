#pragma once

#include <QObject>
#include <QString>
#include "lightsensor.h"
#include "payment.h"
#include "coinsensor.h"


using std::pair;
using std::array;

class Simulation;

enum class coinDestination {cashRegister = 0, hopper = 1};

class CoinSupply : public QObject{
    Q_OBJECT
public:
    CoinSupply(Simulation* m_simulation, array<CoinSensor*, 6> m_coinSensors, array<LightSensor*, 2> m_coinLightSensors, QObject* parent = nullptr);
    ~CoinSupply();

    void storeCoin(coinType);

    std::map<coinType, unsigned int> updatedCoinBalance();
    bool releaseCoin(coinType);

signals:
    void coinStuck(QString);
    void coinStorageFailed(coinType);
    void coinStoredSuccessfully(coinType);

private slots:
    void onCoinDetectionSucccess(bool);

private:
    std::map<coinType, unsigned int> availableCoinBalance;
    std::map<coinType, unsigned int> idealCoinBalance;
    std::map<coinType, unsigned int> hopperCoinBalance;
    void countAvailableCoins();

    void checkCoinSupply();

    coinDestination l_coinDestination;
    bool moveCoin(coinDestination destination);//, coinType coin);
    void closeAllInternalCoinSlots();
    void openRelevantCoinSlots(coinDestination);

    bool openCashRegistry, openHopper;

    int getRemainingCoinSupply();

    Simulation* m_simulation;

    CoinSensor * sensEUR2;
    CoinSensor * sensEUR1;
    CoinSensor * sensEUR05;
    CoinSensor * sensEUR02;
    CoinSensor * sensEUR01;
    CoinSensor * sensEUR005;
    LightSensor * sensHopper;
    LightSensor * sensCoinSupply;

    std::map<coinType, CoinData> m_knownCoins;
    bool coinDetectionSuccess;

};
