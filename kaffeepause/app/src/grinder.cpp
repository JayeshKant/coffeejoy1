#include "grinder.h"

Grinder::Grinder(CoffeeStateMachine* m_coffeeStateMachine, CoffeeWaiter* m_coffeeWaiter, Simulation* m_simulation, QObject* parent)
    : QObject(parent), m_coffeeWaiter(m_coffeeWaiter), m_simulation(m_simulation), m_coffeeStateMachine(m_coffeeStateMachine) {


    connect(m_coffeeWaiter, &CoffeeWaiter::grindCoffee, this, &Grinder::grind);
    connect(m_simulation, &Simulation::grindedBeans, this, &Grinder::onGrindedBeans);
}

Grinder::~Grinder() {}

void Grinder::grind() {
    setBeansInformation();
    grindBeans();
}

void Grinder::reset(){
    qDebug("Grinder::reset");
    this->grindRate = 4;
    this->amountBeans = 0;
    this->typeBeans = beans::espresso;
}

void Grinder::setBeansInformation() {
    // get amount Beans from type of beans, reduce in siumlator and start grinding
    this->amountBeans = m_coffeeWaiter->getAmountBeans();
    qDebug() << "Grinder::getBeans amountBeans: " << this->amountBeans;
    this->typeBeans = m_coffeeWaiter->getTypeBeans();
    qDebug() << "Grinder::getBeans typeBeans: " << (int)this->typeBeans;
}

void Grinder::grindBeans() {
    qDebug() << "Grinding Beans";
    m_simulation->grindBeans(grindRate, typeBeans, this->amountBeans);
}

void Grinder::onGrindedBeans(){
    m_coffeeStateMachine->trigger(event::GRINDING_COMPLETET);
    emit grindingComplete();
}
