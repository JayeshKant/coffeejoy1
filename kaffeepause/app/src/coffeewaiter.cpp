#include "coffeewaiter.h"
#include "simulation.h"
#include "grinder.h"
#include "coffeeselection.h"
#include "coffeestatemachine.h"

#include "coffeetypes.h"

CoffeeWaiter::CoffeeWaiter(
    Payment* m_payment,
    LightSensor* m_cupInserted,
    Thermoblock* m_thermoblock,
    Simulation* m_simulation,
    CoffeeStateMachine* m_coffeeStateMachine,
    CoffeeSelection* m_coffeeSelection,
    QObject* parent)
    : QObject(parent),
    m_coffeeSelection(m_coffeeSelection),
    m_coffeeStateMachine(m_coffeeStateMachine),
    m_simulation(m_simulation),
    m_thermoblock(m_thermoblock),
    m_payment(m_payment),
    m_cupInserted(m_cupInserted) {

    connect(m_thermoblock, &Thermoblock::waterTemperatureOK, this, &CoffeeWaiter::onWaterTemperatureOK);
    connect(m_payment, &Payment::paymentDone, this, &CoffeeWaiter::startCoffee);
    connect(m_cupInserted, &LightSensor::cupInserted, this, &CoffeeWaiter::onCupInserted);

}

CoffeeWaiter::~CoffeeWaiter() {}


void CoffeeWaiter::reset() {
    coffeeIntensity = intensity::medium;
    typeCoffee = coffee::undefined;
    setAmountWater();
    setBeans();
    setAmountMilk();
}

beans CoffeeWaiter::getTypeBeans() {
    return this->typeBeans;
}

int CoffeeWaiter::getAmountWater() {
    return this->amountWater;
}

coffee CoffeeWaiter::getTypeCoffee(){
    return this->typeCoffee;
}

void CoffeeWaiter::setAmountWater() {
    switch (typeCoffee) {
    case coffee::cappuccino: //intentional without break
    case coffee::espresso:
        this->amountWater = 25;
        break;
    case coffee::crema:
        this->amountWater = 150;
        break;
    default:
        // TODO fehlerbehandlung
        this->amountWater = 0;
        break;
    }
}



void CoffeeWaiter::setAmountMilk(){
    if (typeCoffee == coffee::cappuccino) {
        this->amountMilk = 60;
    } else {
        this->amountMilk = 0;
    }
}

int CoffeeWaiter::getAmountMilk(){
    return this->amountMilk;
}


int CoffeeWaiter::getAmountBeans() {
    return this->amountBeans;
}


void CoffeeWaiter::setBeans() {
    switch (typeCoffee) {
    case coffee::cappuccino: //intentional without break
    case coffee::espresso:
        this->typeBeans = beans::espresso;
        switch (coffeeIntensity) {
        case intensity::light:
            this->amountBeans = 8;
            break;
        case intensity::medium:
            this->amountBeans = 16;
            break;
        case intensity::strong:
            this->amountBeans = 20;
            break;
        default:
            break;
        }
        break;

    case coffee::crema:
        this->typeBeans = beans::crema;
        switch (coffeeIntensity) {
        case intensity::light:
            this->amountBeans = 8;
            break;
        case intensity::medium:
            this->amountBeans = 12;
            break;
        case intensity::strong:
            this->amountBeans = 16;
            break;
        default:
            break;
        }
    default:
        break;
    }
}


void CoffeeWaiter::setGrinder(Grinder* m_grinder){
    this->m_grinder = m_grinder;
    connect(m_grinder, &Grinder::grindingComplete, this, [this]() {
        heatUpWater(mode::brewing);
    });
}

void CoffeeWaiter::startCoffee(){
    qDebug() << "Start coffee making process";
    this->coffeeIntensity = m_coffeeSelection->getCoffeeIntensity();
    this->typeCoffee = m_coffeeSelection->getTypeCoffee();
    setAmountWater();
    setBeans();
    setAmountMilk();
    m_coffeeStateMachine->trigger(event::PREPARE_COFFEE);
    emit grindCoffee();
}


void CoffeeWaiter::heatUpWater(mode thermoBlockMode){
    qDebug() << "CoffeeWaiter::heatUpWater setting mode to " << (int)thermoBlockMode;
    m_thermoblock->setCurrentMode(thermoBlockMode);
    // m_thermoblock->setTargetTemperature(thermoBlockMode);
}


void CoffeeWaiter::onWaterTemperatureOK(){
    m_coffeeStateMachine->trigger(event::WATER_TEMPERATURE_OK);
}

void CoffeeWaiter::onCupInserted(){
    m_coffeeStateMachine->trigger(event::CUP_INSERTED);
    emit startBrewing();
}

intensity CoffeeWaiter::getCoffeeIntensity() {
    return this->coffeeIntensity;
}
void CoffeeWaiter::setCoffeeIntensity(intensity coffeeIntensity) {
    this->coffeeIntensity = coffeeIntensity;
}

void CoffeeWaiter::setTypeCoffee(coffee typeCoffee){
    this->typeCoffee = typeCoffee;
}








