#include "touchhandler.h"
#include "QDebug"



const map<state, vector<Buttons>> TouchHandler::knownButtons = {

        {state::preparationStart, {
            {350, 450, 250, 350, buttons::start}
        }},

        {state::readySelectCoffee, {
            {137, 238, 100, 500, buttons::crema},
            {262, 363, 100, 500, buttons::espresso},
            {387, 488, 100, 500, buttons::cappuccino},
            {512, 613, 300, 500, buttons::maintenance}
        }},

        {state::confirmIntensity, {
            {137, 238, 100, 500, buttons::light},
            {262, 363, 100, 500, buttons::medium},
            {387, 488, 100, 500, buttons::strong},
            {512, 613, 300, 500, buttons::abort}
        }},

        {state::makeCoffee, {
            {512, 613, 300, 500, buttons::abort}}
        },

        {state::brewCoffee, {
            {512, 613, 300, 500, buttons::abort}
        }},

        {state::maintenance, {
            {200, 300, 100, 500, buttons::decalcification},
            {325, 425, 100, 500, buttons::shutdown},
            {450, 550, 300, 500, buttons::abort}
         }}
};


TouchHandler::TouchHandler(CoffeeStateMachine* m_stateMachine,
                           TouchScreen* m_touchScreen,
                           QObject* parent)
    : QObject(parent),
    m_stateMachine(m_stateMachine),
    m_touchScreen(m_touchScreen) {

    connect(m_touchScreen, &TouchScreen::touchEvent, this, &TouchHandler::onTouchEvent);
    connect(this, &TouchHandler::buttonPressed, this, &TouchHandler::onButtonPressed);


    qDebug() << "TouchHandler: created and connected to TouchScreen";


}

TouchHandler::~TouchHandler(){

}

void TouchHandler::reset() {
    qDebug("TouchHandler::reset");
    currentState = m_stateMachine->getCurrentState();
    pressedButton = buttons::undefined;
}


void TouchHandler::setCurrentButton(buttons button){
    qDebug() << "TouchHandler::setCurrentButton() ->" << static_cast<int>(button);
    this->pressedButton = button;
}

buttons TouchHandler::getCurrentButton(){
    return this->pressedButton;
}

bool TouchHandler::checkTouchEvent(int x, int y, Buttons button) {
    if ( x > button.beginX && x < button.endX ) {
        if (y > button.beginY && y < button.endY) {
            qDebug() << "TouchHandler::checkTouchEvent() hit button =" << static_cast<int>(button.name);
            setCurrentButton(button.name);
            return true;
        }
    }
    return false;
}


void TouchHandler::onTouchEvent(){
    qDebug() << "TouchHandler::onTouchEvent()";
    currentState = m_stateMachine->getCurrentState();
    qDebug() << "  currentState =" << static_cast<int>(currentState);

    int x = m_touchScreen->getXY().first;
    int y = m_touchScreen->getXY().second;
    qDebug() << "  touch coords =" << x << y;
    auto it = knownButtons.find(currentState);

    if (it != knownButtons.end()) {
        for (const Buttons& button : it->second) {
            if(checkTouchEvent(x, y, button)){
                qDebug() << "  emitting buttonPressed for" << static_cast<int>(pressedButton);
                emit buttonPressed();
            }
        }
    }
}

vector<Buttons> TouchHandler::getKnownButtons(){
    return knownButtons.find(m_stateMachine->getCurrentState())->second;
}


void TouchHandler::onButtonPressed(){
 qDebug() << "TouchHandler::onButtonPressed() pressedButton =" << static_cast<int>(pressedButton);
    switch(pressedButton){
    case buttons::start:
        //emit signal to start the machine
        emit startUp();
        break;

    case buttons::abort:
        // TODO Return Change
        // restartButtonIsClicked();
        // m_stateMachine->trigger(event::ABORT_REQUESTED);

        // TODO proper abort protocoll
        break;

    case buttons::maintenance:
        //emit signal to start Maintenance
        // emit m_stateMachine->maintenanceRequested();
        m_stateMachine->trigger(event::MAINTENANCE_REQUESTED);
        break;

    case buttons::decalcification:
        //emit signal to start Maintenance
        // emit m_stateMachine->decalcificationRequested();
        m_stateMachine->trigger(event::DECALCIFICATION_REQUESTED);
        break;

    case buttons::crema:
        emit coffeeSelected(coffee::crema);
        break;

    case buttons::espresso:
        emit coffeeSelected(coffee::espresso);
        break;

    case buttons::cappuccino:
        emit coffeeSelected(coffee::cappuccino);
        break;

    case buttons::light:
        emit intensityConfirmed(intensity::light);
        break;

    case buttons::medium:
        emit intensityConfirmed(intensity::medium);
        break;

    case buttons::strong:
        emit intensityConfirmed(intensity::strong);
        break;

    case buttons::shutdown:
        m_stateMachine->trigger(event::SHUTDOWN_REQUESTED);
        emit shutdownRequested();
        break;
    case buttons::undefined:
        break;
    }
}

