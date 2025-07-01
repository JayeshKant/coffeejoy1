#pragma once

#include <QObject>
#include <vector>

#include "coffeeselection.h"

// Forward declarations
class CoinChecker;
class CoffeeStateMachine;
class CoinSensor;
class CoinSupply;
class Simulation;

using std::map;
using std::vector;

struct CoinData {
    int diameterMicrometer;
    int widthMicrometer;
    int weightMilligram;
    int magneticProperty;

    static constexpr int numberOfProperties = 4; // NEEDS TO BE UPDATED WHEN NEW PROPERTY IS INSERTED!

    int at(int index) const {
        switch(index) {
        case 0: return diameterMicrometer;
        case 1: return widthMicrometer;
        case 2: return weightMilligram;
        case 3: return magneticProperty;
        default: throw std::out_of_range("Invalid index");
        }
    }
};


enum class coinType {
    EUR005 = 5, EUR01 = 10, EUR02 = 20, EUR05 = 50, EUR1 = 100, EUR2 = 200, INVALID, RESET
};

// function to iterate over coinType
inline std::vector<coinType> availableCoins() {
    return {
        coinType::EUR2,
        coinType::EUR1,
        coinType::EUR05,
        coinType::EUR02,
        coinType::EUR01,
        coinType::EUR005
    };
}


class Payment  : public QObject {
    Q_OBJECT

public:
    Payment(Simulation* m_simulation, CoffeeSelection* m_coffeeSelection, QObject* parent = nullptr);
    ~Payment();

    void setCoinChecker(CoinChecker* m_coinChecker);

    void setCoffeeStateMachine(CoffeeStateMachine* m_coffeeStateMachine);

    bool getCoinIsInserted();
    void setCoinIsInserted(bool coinIsInserted);

    void setCoinSupply(CoinSupply* coinsupply);

    void setInsertedCoinData(CoinData insertedCoinData);
    CoinData getInsertedCoinData();

    int getCheckedInsertedCoinValue();

    void setCoinIsValid(bool coinIsValid);
    bool getCoinIsValid();

    void setCurrentAmountPaid();
    int getCurrentAmountPaid();

    bool checkNeededAmountReached();


    void manageInsertedObject(int[4]); // still needed?
    void coinValue(int coin);

    void reset();

signals:

    void waitingForCoin(); // Erstes Mal automatisch aus transition dann in Payment
    void coinBooked(); //Payment
    void requiredAmountReached(); //Payment
    void paymentDone(); //Payment
    void newBalance();


public slots: //needs to be Public, CoffeeStateMachine needs to connect with current state
    void onCoinBooked();
    void onRequiredAmountReached();
    void onStorageSuccess(coinType);
    void onStorageFailed(coinType);

private:
    Simulation* m_simulation;
    CoinChecker* m_coinChecker;
    CoinSensor* m_coinSensor;
    CoffeeSelection* m_coffeeSelection;
    CoffeeStateMachine* m_coffeeStateMachine;
    CoinSupply* m_coinSupply;

    int amountPaid = 0;
    int amountChange = 0;

    map<coinType, int> coinReturnMap;

    bool coinIsInserted = false;
    CoinData insertedCoinData;
    bool coinIsValid = false;

    void calculateChange(int);

    int controlledRelease(coinType, int counterCoins);
    int returnChange(int changeAmount);




protected:
    int realCoinSupply[6];

};

