#include "simulation.h"
#include "coinsensor.h"
#include "pumpcontrol.h"
#include "simulation.h"
#include "touchhandler.h"

Simulation::Simulation(TouchScreen* m_touchScreen, CoinSensor* m_coinSensor, Pump* m_pump, Valve* m_freshWaterSupplyValve, QObject* parent)
    : QObject(parent), m_touchScreen(m_touchScreen), m_coinSensor(m_coinSensor), m_pump(m_pump), m_freshWaterSupplyValve(m_freshWaterSupplyValve) {
    // initial Values
    int initialValue = 50;

    qDebug() << "Simulation: Initial Values for coinLevelMap";
    this->coinLevelMap = {
        {coinType::EUR005, m_coinSensor->knownCoins().at(coinType::EUR005).widthMicrometer * initialValue}, // useknownCoins
        {coinType::EUR01, m_coinSensor->knownCoins().at(coinType::EUR01).widthMicrometer * initialValue},
        {coinType::EUR02, m_coinSensor->knownCoins().at(coinType::EUR02).widthMicrometer * initialValue},
        // {coinType::EUR05, m_coinSensor->knownCoins().at(coinType::EUR05).widthMicrometer * initialValue},
        {coinType::EUR05, m_coinSensor->knownCoins().at(coinType::EUR05).widthMicrometer * 0},
        // {coinType::EUR1, m_coinSensor->knownCoins().at(coinType::EUR1).widthMicrometer * initialValue},
        {coinType::EUR1, m_coinSensor->knownCoins().at(coinType::EUR1).widthMicrometer * 0},
        {coinType::EUR2, m_coinSensor->knownCoins().at(coinType::EUR2).widthMicrometer * initialValue}
    };

    qDebug() << "Simulation: Initial Values for coinReturnMap";
    this->coinReturnMap = {
        {coinType::EUR005, 0},
        {coinType::EUR01, 0},
        {coinType::EUR02, 0},
        {coinType::EUR05, 0},
        {coinType::EUR1, 0},
        {coinType::EUR2, 0}
    };



    //Start Values TODO from file
    qDebug() << "Simulation: Initial Values for lightSensorsStatusMap";
    this->lightSensorsStatusMap = {
        {lightSensors::hopper, detection::lightReceived},
        {lightSensors::coinSupply, detection::lightReceived},
        {lightSensors::espressoBeanLevel, detection::lightBlocked},
        {lightSensors::cremaBeanLevel, detection::lightBlocked},
        {lightSensors::wasteDisposale, detection::lightReceived},
        {lightSensors::cupInserted, detection::lightReceived},
        {lightSensors::freshWaterLevel, detection::lightBlocked}
    };

    qDebug() << "Simulation: Initial Values for beansLevelMap";
    this->beansLevelMap = {
        {beans::crema, 2000}, // 2kg capacity
        {beans::espresso, 2000}
    };

    qDebug() << "Simulation: Initial Values for m_coinLightSensors";
    this->m_coinLightSensors = {
        {lightSensors::hopper, nullptr},
        {lightSensors::coinSupply, nullptr},
        {lightSensors::espressoBeanLevel, nullptr},
        {lightSensors::cremaBeanLevel, nullptr},
        {lightSensors::wasteDisposale, nullptr},
        {lightSensors::cupInserted, nullptr},
        {lightSensors::freshWaterLevel, nullptr}
    };

    qDebug() << "Simulation: Initialize Timer for Pressure";
    pressureTimer = new QTimer(this);
    connect(pressureTimer, &QTimer::timeout, this, &Simulation::updatePressure);

    qDebug() << "Simulation: Initialize Timer for Waterpump";
    pumpWaterTimer = new QTimer(this);
    connect(pumpWaterTimer, &QTimer::timeout, this, &Simulation::reduceWater);

    qDebug() << "Simulation: Initialize Timer for Grinder";
    grinderTimer = new QTimer(this);
    connect(grinderTimer, &QTimer::timeout, this, &Simulation::reduceBeans);

    qDebug() << "Simulation: Initialize Timer for Milkpump";
    pumpMilkTimer = new QTimer(this);
    connect(pumpMilkTimer, &QTimer::timeout, this, &Simulation::reduceMilk);

    qDebug() << "Simulation: Initialize and Start Timer for LightSensor updating";
    updateLightSensorTimer = new QTimer(this);
    connect(updateLightSensorTimer, &QTimer::timeout, this, &Simulation::updateLightSensors);

    qDebug() << "Simulation: Initialize and Start Timer for MilkHeatingEnvironment";
    updateMilkHeatingTimer = new QTimer(this);
    connect(updateMilkHeatingTimer, &QTimer::timeout, this, &Simulation::heatMilk);

    qDebug() << "Simulation: Initialize and Start Timer for fillFreshWaterValveTimer";
    fillFreshWaterValveTimer = new QTimer(this);
    connect(fillFreshWaterValveTimer, &QTimer::timeout, this, &Simulation::fillFreshWaterValve);



}

Simulation::~Simulation() {}

void Simulation::heatMilk(){
    milkTemperature += 1;
    qDebug() << "Simulation::heatMilk The environment increased the milk "
                "temperature +1 degree. Temperature now: " <<milkTemperature;
}


void Simulation::start(){
    pressureTimer->start(1000);
    fillFreshWaterValveTimer->start(1000);
    updateMilkHeatingTimer->start(20000); // TODO stop when shutting down
    updateLightSensorTimer->start(3000); // TODO stop when shutting down

}

void Simulation::reset(){
    qDebug("Simulation::reset");
    this->coinReturnMap = {
        {coinType::EUR005, 0},
        {coinType::EUR01, 0},
        {coinType::EUR02, 0},
        {coinType::EUR05, 0},
        {coinType::EUR1, 0},
        {coinType::EUR2, 0}
    };


    //Start Values TODO from file
    qDebug() << "Simulation: Initial Values for lightSensorsStatusMap";
    this->lightSensorsStatusMap = {
        {lightSensors::hopper, detection::lightReceived},
        {lightSensors::coinSupply, detection::lightReceived},
        {lightSensors::espressoBeanLevel, detection::lightBlocked},
        {lightSensors::cremaBeanLevel, detection::lightBlocked},
        {lightSensors::wasteDisposale, detection::lightReceived},
        {lightSensors::cupInserted, detection::lightReceived},
        {lightSensors::freshWaterLevel, detection::lightBlocked}
    };


    pumpWaterTimer->stop();
    grinderTimer->stop();
    pumpMilkTimer->stop();

    selectedBeans = beans::espresso;
    requiredAmountBeans = 0;
    grindRate = 0;
    beansGrinded = 0;

    //Thermoblock

    currentTemperature = 60;
    waterPumped = 0;
    flowRate = 0;

    waterInMlDispensed = 0;
    waterNeeded = 0;

    //MilkUnit

    milkInMlDispensed = 0;
    milkNeeded = 0;
    flowRateWater = 0;
    flowRateMilk = 0;

    //PumpControl

    currentPressure = 0; // when starting always 0 in mbar

    //Waste

    wasteWater = 0; // max 1500ml

    milkTemperature = 20;
}


void Simulation::setPumpControl(PumpControl* m_pumpControl){
    qDebug() << "Simulation: set PumpControl pointer";
    this->m_pumpControl = m_pumpControl;
}

void Simulation::setTouchHandler(TouchHandler* m_touchHandler){
    qDebug() << "Simulation::setTouchHandler";
    this->m_touchHandler = m_touchHandler;
}

void Simulation::onEurButtonClicked(coinType buttonClicked){

    static const map<coinType, CoinData > buttonToCharacteristics {
        {  coinType::EUR005,    {2125, 167, 392, 0} },
        {  coinType::EUR01,     {1975, 193, 410, 1} },
        {  coinType::EUR02,     {2225, 214, 574, 2} },
        {  coinType::EUR05,     {2425, 238, 780, 3} },
        {  coinType::EUR1,      {2325, 167, 750, 4} },
        {  coinType::EUR2,      {2575, 167, 850, 5} },
        {  coinType::INVALID,   {2125, 167, 250, 6} }
    };

    auto it = buttonToCharacteristics.find(buttonClicked);



    if (it != buttonToCharacteristics.end()){
        const CoinData& coinClicked = it->second;

        qDebug() << "Gefundener Button:" << (int)it->first;
        qDebug() << "CoinData:"
                 << "Diameter (µm):" << coinClicked.diameterMicrometer
                 << "Width (µm):"    << coinClicked.widthMicrometer
                 << "Weight (mg):"   << coinClicked.weightMilligram
                 << "Magnetic:"      << coinClicked.magneticProperty;

        emit coinInserted(coinClicked);
    }
}


void Simulation::onButtonTouched(buttons buttonClicked){

    int x = 0;
    int y = 0;

    auto it = m_touchHandler->getKnownButtons();

    // Random Zahl in festgelegtem Bereich von Button wird generiert und an TouchScreen übergeben
   for (const Buttons& button : it) {
            if (button.name == buttonClicked) {
                x = QRandomGenerator::global()->bounded(button.beginX + 1, button.endX); // beginX <= x < endX
                y = QRandomGenerator::global()->bounded(button.beginY + 1, button.endY);
            }
        }

    m_touchScreen->setXY(x, y);

}


void Simulation::cupInserted(bool status){
    if (status) {
        setDetection(lightSensors::cupInserted, detection::lightBlocked);
    } else {
        setDetection(lightSensors::cupInserted, detection::lightReceived);
    }
}



// COINS

void Simulation::setCoinFlap(coinDestination destination){
    switch (destination) {
    case coinDestination::cashRegister:
        if(cashRegisterBlocker == statusOfBlocker::closed){
            cashRegisterBlocker = statusOfBlocker::opened;}
        else if(cashRegisterBlocker == statusOfBlocker::opened){
            cashRegisterBlocker = statusOfBlocker::closed;}
        else{
        qDebug()<<"undefined coinDestination for setCoinFlap for CashRegister.";
        }
        break;
    case coinDestination::hopper:
        if(hopperBlocker == statusOfBlocker::closed){
            hopperBlocker = statusOfBlocker::opened;}
        else if(hopperBlocker == statusOfBlocker::opened){
            hopperBlocker = statusOfBlocker::closed;}
        else{
            qDebug() << "undefined coinDestination for setCoinFlap for hopper.";
        }
        break;
    }
}

 void Simulation::addCoin(coinType type, int width){
     this->coinLevelMap.at(type) += width;
}

 void Simulation::removeCoin(coinType type, int width){
     this->coinLevelMap.at(type) -= width;
 }

statusOfBlocker Simulation::getStatusOfBlocker(coinDestination l_coinDestination){
    switch (l_coinDestination) {
    case coinDestination::cashRegister:
        return cashRegisterBlocker;
        break;
    case coinDestination::hopper:
        return hopperBlocker;
        break;
    default: qDebug()<<"getStatusofBlocker unkown destination";
        break;
    }
}

void Simulation::closeAllCoinSupplyBlocker(){
    cashRegisterBlocker = statusOfBlocker::closed;
    hopperBlocker = statusOfBlocker::closed;
}

void Simulation::setCoinLevel(coinType type, int levelInMicrometer){
// The qFatal messages should be prevented by the coinSupply-Logic only here to indicate errors!
    if(coinLevelMap.at(type) < levelInMicrometer){
        if(coinLevelMap.at(type) < 350 * m_coinSensor->knownCoins().at(type).widthMicrometer){
            qFatal() << "Simulation::setCoinLevel Maximum amount of Coins reached! ERROR" << (int)type << ". Adding " << levelInMicrometer;
        }
        qDebug() << "Simulation::setCoinLevel increasing CoinLevelMap at" << (int)type << ". Adding " << levelInMicrometer;
    }else if(coinLevelMap.at(type) > levelInMicrometer){
        if(coinLevelMap.at(type) == 0){
            qFatal() << "Simulation::setCoinLevel no Coins of type" << (int)type << " to pay out! ERROR!";
        }
        qDebug() << "Simulation::setCoinLevel decreasing CoinLevelMap at" << (int)type << ". new level " << levelInMicrometer/m_coinSensor->knownCoins().at(type).widthMicrometer;
    }else{
        qDebug() << "Simulation::setCoinLevel Oh  boy, you tried to set the same CoinLevel, check your code.";
    }

    this->coinLevelMap.at(type) = levelInMicrometer;
}

int Simulation::getCoinLevel(coinType type){
    qDebug() << "Simulation: return coinLevelMap at" << (int)type;
    return this->coinLevelMap.at(type);
}

void Simulation::setCoinReturn(coinType type){
    qDebug() << "Simulation: increasing coinReturnMap at" << (int)type
             << ". Adding 1 Coin. ";
            // TODO add count of coins of type get width and then divide
    this->coinReturnMap.at(type) += 1;
}

int Simulation::getCoinReturn(coinType type){
    qDebug() << "Simulation: return coinReturnMap at" << (int)type;
    return this->coinReturnMap.at(type);
}

std::map<coinType, int> Simulation::getCoinLevelMap(){
    return this->coinLevelMap;
}

void Simulation::setCoinLightSensorInSimulation(lightSensors name, LightSensor* lightSensorPtr){
    m_coinLightSensors[name]=lightSensorPtr;

    connect(lightSensorPtr, &LightSensor::waitingForDetectedCoin, this, &Simulation::onWaitingForDetectedCoin);
}


void Simulation::onWaitingForDetectedCoin(lightSensors sensor){
    QTimer::singleShot(500, this, [this, sensor](){
        toggleDetection(sensor);
    });
    QTimer::singleShot(1000, this, [this, sensor](){
        toggleDetection(sensor);
    });
}

detection Simulation::getDetection(lightSensors sensor){
    // qDebug() << "Simulation::getDetection return lightSensorsStatusMap at" << (int)sensor;
    return this->lightSensorsStatusMap.at(sensor);
}

void Simulation::setDetection(lightSensors sensor, detection newValue){
    qDebug() << "Simulation::setDetection setting lightSensorsStatusMap at" << (int)sensor << ". New Value: " << (int)newValue;
    this->lightSensorsStatusMap.at(sensor) = newValue;
}

void Simulation::toggleDetection(lightSensors sensor) {
    qDebug() << "Simulation::toggleDetection called – sensor:" << (int)sensor
             << "previous:" << (int)lightSensorsStatusMap.at(sensor)
             << "new:" << (int)(lightSensorsStatusMap.at(sensor) == detection::lightBlocked ? detection::lightReceived : detection::lightBlocked);

    if (lightSensorsStatusMap.at(sensor) == detection::lightBlocked) {
        setDetection(sensor, detection::lightReceived);
    } else {
        setDetection(sensor, detection::lightBlocked);
    }
}


void Simulation::setEnoughChange(bool enoughChange){
    this->enoughChange = enoughChange;
}

bool Simulation::getEnoughChange(){
    return enoughChange;
}





//COFFEBEANS / GRINDING
//TODO give time and not grind/flowRate

void Simulation::grindBeans(int grindRate, beans selectedBeans, int requiredAmountBeans){
    this->requiredAmountBeans = requiredAmountBeans;
    this->selectedBeans = selectedBeans;
    this->grindRate = grindRate;

    grinderTimer->start(1000);
}

void Simulation::reduceBeans(){

    if (requiredAmountBeans <= beansGrinded) {
        grinderTimer->stop();
        emit grindedBeans();
        qDebug() << "Simulation::setCoffeeBeansLevel reducing beansLevelMap at " << (int)this->selectedBeans << " by " << this->requiredAmountBeans;
        return;
    }
    if (this->beansLevelMap.at(this->selectedBeans) >= grindRate) {
        this->beansLevelMap.at(this->selectedBeans) -= grindRate; //reduce Beans
        beansGrinded += grindRate;
        emit grindingProgress(beansGrinded, requiredAmountBeans);
        qDebug() << "Beans" << (int)this->selectedBeans << "reduced by" << grindRate << "g. Grinded:" << beansGrinded << "g";
    } else {
        grinderTimer->stop();
        emit notEnoughBeans();
        return;
    }
}


void Simulation::resetCoffeeBeansLevel(beans typeBeans){
    qDebug() << "Simulation::resetCoffeeBeansLevel resetting beansLevelMap at " << (int)typeBeans << " to " << maxCoffeeBeansInGramm;

    this->beansLevelMap.at(typeBeans) = maxCoffeeBeansInGramm;
}


int Simulation::getCoffeeBeansLevel(beans typeBeans){
    qDebug() << "Simulation::getCoffeeBeansLevel beansLevelMap at " << (int)typeBeans;
    return this->beansLevelMap.at(typeBeans);
}

void Simulation::setMilkTemperature(int milkTemperature){
    qDebug() << "Simulation::setMilkTemperature to " << milkTemperature;
    this->milkTemperature = milkTemperature;
}

int Simulation::getMilkTemperature(){
    // qDebug() << "Simulation::getMilkTemperature to " << this->milkTemperature;
    return this->milkTemperature;
}

void Simulation::updateLightSensors(){
    if(beansLevelMap.at(beans::crema) < 15){
        lightSensorsStatusMap.at(lightSensors::cremaBeanLevel) = detection::lightReceived;
    }

    if(beansLevelMap.at(beans::espresso) < 20){
        lightSensorsStatusMap.at(lightSensors::espressoBeanLevel) = detection::lightReceived;
    }

    if(currentMilkAmount < 60){
        lightSensorsStatusMap.at(lightSensors::milkLevel) = detection::lightReceived;
    }

    if(beansGrinded > 2500){
        lightSensorsStatusMap.at(lightSensors::wasteDisposale) = detection::lightBlocked;
    }

    if(wasteWater >= 1500){
        lightSensorsStatusMap.at(lightSensors::wasteDisposale) = detection::lightBlocked;
    }

    if(currentWaterAmount >= maxWaterAmount){
        lightSensorsStatusMap.at(lightSensors::freshWaterLevel) = detection::lightBlocked;
    }
}


int Simulation::getBeansGrinded(){
    return this->beansGrinded;
}





// WATER AND MILK

//For Thermoblock

void Simulation::setCurrentTemperature(int newTemperature){
    this->currentTemperature = newTemperature;
}
int Simulation::getCurrentTemperature(){
    return this->currentTemperature;
}






// For Brewing

void Simulation::pumpWater(int waterNeeded, int flowRate){
    this->waterNeeded = waterNeeded;
    this->flowRate = flowRate;
    this->waterInMlDispensed = 0;

    if (waterNeeded + 15 >= currentWaterAmount) {
        emit notEnoughWater();
        return;
    }

    pumpWaterTimer->start(500); // alle 0,5 s
}

void Simulation::reduceWater(){
    if (waterNeeded <= waterInMlDispensed) {
        pumpWaterTimer->stop();
        emit waterDispensed();
        return;
    }

    if (currentWaterAmount >= flowRate) {
        currentWaterAmount -= flowRate; //reduce Water in tank
        waterInMlDispensed += flowRate;
        qDebug() << "Water reduced by" << flowRate << "ml. Dispensed:" << waterInMlDispensed << "ml";
    } else {
        pumpWaterTimer->stop();
        emit notEnoughWater();
        return;
    }

}

int Simulation::getCurrentWaterAmount(){
    return this->currentWaterAmount;
}


void Simulation::fillFreshWaterValve(){
    if(m_freshWaterSupplyValve->getValveState() == valveState::open){
        currentWaterAmount += 5;
    }
}



int Simulation::getWaterDispensed(){
    return this->waterInMlDispensed;
}



//For PumpControl

void Simulation::setCurrentPressure(int newPressure){
    this->currentPressure = newPressure;
}

int Simulation::getCurrentPressure(){
    return this->currentPressure;
}

void Simulation::updatePressure(){ // if pump is running, add pressure, if not, decrease pressure. Warning if pressure to high
    if(m_pump->isRunning()) {
        int targetPressure = m_pumpControl->getTargetPressure();

        if (currentPressure < targetPressure) {
            currentPressure += 1000;
            //qDebug() << "Simulation::updatePressure increased Pressure, new Value: " << currentPressure;
        } else if (currentPressure > targetPressure){
            qDebug() << "Simulation::updatePressure Pressure to High! Value: " << currentPressure;
            emit pressureToHigh();
        }
    } else {
        if (currentPressure > 0){
            currentPressure -= 1000;
            qDebug() << "Simulation::updatePressure reduced Pressure, new Value: " << currentPressure;
        }
    }


}











// for Milk


void Simulation::pumpMilk(int milkNeeded, int flowRateMilk, int flowRateWater){
    this->milkNeeded = milkNeeded;
    this->flowRateWater = flowRateWater;
    this->flowRateMilk = flowRateMilk;
    this->milkInMlDispensed = 0;


    pumpMilkTimer->start(500); // alle 0.5 s


}

void Simulation::reduceMilk(){
    if (milkNeeded <= milkInMlDispensed) {
        pumpMilkTimer->stop();
        emit milkDispensed();
        return;
    }

    if (currentMilkAmount >= flowRateMilk) {
        currentMilkAmount -= flowRateMilk; //reduce Milk in tank
        currentWaterAmount -= flowRateWater;
        milkInMlDispensed += flowRateMilk;
        qDebug() << "Milk reduced by" << flowRate << "ml. Disepensed:" << milkInMlDispensed << "ml";
    } else {
        pumpMilkTimer->stop();
        emit notEnoughMilk();
        return;
    }

}

int Simulation::getCurrentMilkAmount(){
    return this->currentMilkAmount;
}


int Simulation::getDispencedAmountMilk(){
    return this->milkInMlDispensed;
}
