#include "coffeeselection.h"
#include "touchhandler.h"

#include <QDebug>


CoffeeSelection::CoffeeSelection(QObject* parent) : QObject(parent) {
    // Initialize generalAvailability or other members as needed
    // For now, we'll just zero-initialize the generalAvailability array
    for (int &value : generalAvailability) {
        value = 0;
    }
}

CoffeeSelection::~CoffeeSelection() {
}

void CoffeeSelection::reset(){
    qDebug("CoffeeSelection::reset");
    this->priceInCent = 0;
    this->typeCoffee = coffee::undefined;
    this->chosenIntensity = intensity::medium; //standard is medium
}

void CoffeeSelection::setTouchHandler(TouchHandler* m_touchHandler){
    this->m_touchHandler = m_touchHandler;
    connect(m_touchHandler, &TouchHandler::coffeeSelected, this, &CoffeeSelection::setCoffee);
    connect(m_touchHandler, &TouchHandler::intensityConfirmed, this, &CoffeeSelection::setCoffeeIntensity);

}

int CoffeeSelection::getPriceInCent() {
    return this->priceInCent;
}

void CoffeeSelection::setPriceInCent() {
    qDebug() << "CoffeeSelection: set Price to ";
    switch (this->typeCoffee) {
    case coffee::crema:
        this->priceInCent = 100;
        qDebug() << "1€";
        break;
    case coffee::espresso :
        this->priceInCent = 200;
        qDebug() << "2€";
        break;
    case coffee::cappuccino :
        this->priceInCent = 300;
        qDebug() << "3€";
        break;
    case coffee::undefined :
        this->priceInCent = 0;
        qDebug() << "0€";
        break;
    default:
        //TODO Errormessage catch unknown coffee price
        break;
    }
}


coffee CoffeeSelection::getTypeCoffee() {
    return this->typeCoffee;
}


void CoffeeSelection::setTypeCoffee(coffee typeCoffee) {
    qDebug() << "CoffeeSelection: set CoffeType to: " << (int)typeCoffee;
    qDebug() << "crema = 1, espresso = 2, cappuccino = 3, undefined = 0";
    this->typeCoffee = typeCoffee;
}

intensity CoffeeSelection::getCoffeeIntensity(){
    qDebug() << "CoffeeSelection: return chosenIntensity" << (int)chosenIntensity;
    qDebug() << "strong = 2, medium = 1, light = 0";
    return this->chosenIntensity;
}

void CoffeeSelection::setCoffeeIntensity(intensity chosenIntensity){ //onIntensityConfirmed
    qDebug() << "CoffeeSelection: set chosenIntensity to:" << (int)chosenIntensity;
    qDebug() << "strong = 2, medium = 1, light = 0";
    this->chosenIntensity = chosenIntensity;
}


void CoffeeSelection::setCoffee(coffee typeCoffee){ //onCoffeeSelected
    qDebug() << "CoffeeSelection set type and price";
    this->setTypeCoffee(typeCoffee);
    this->setPriceInCent();
}


//TODO Availability

vector<int> CoffeeSelection::getGeneralAvailability() const {
    return vector<int>(generalAvailability, generalAvailability + 3);
}

void CoffeeSelection::setGeneralAvailability(const int generalAvailability[3]) {
    for (int i = 0; i < 3; ++i) {
        this->generalAvailability[i] = generalAvailability[i];
    }
}


bool CoffeeSelection::confirm() {
    // Implementation for confirm (this is a placeholder)
    return true;
}

void CoffeeSelection::sendConfirmation() {
}
