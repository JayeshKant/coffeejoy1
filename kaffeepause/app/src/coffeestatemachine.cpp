#include "coffeestatemachine.h"
#include "touchhandler.h"

CoffeeStateMachine::CoffeeStateMachine(
    Simulation* m_simulation,
    Payment* m_payment,
    CoinChecker* m_coinChecker,
    CoffeeSelection* m_coffeeSelection,
    CoinSupply* m_coinSupply,
    QObject* parent)
    : QObject(parent),
    m_simulation(m_simulation),
    m_payment(m_payment),
    m_coinChecker(m_coinChecker),
    m_coffeeSelection(m_coffeeSelection),
    m_coinSupply(m_coinSupply){

    connect(this, &CoffeeStateMachine::coffeeReadyToDispense, this, [this]() { trigger(event::COFFEE_READY_TO_TAKEOUT); });
    connect(this, &CoffeeStateMachine::coffeeReadyToTakout, this, [this]() { trigger(event::FINISHED); });

    connect(this, &CoffeeStateMachine::shutdownRequested, this, [this]() { trigger(event::SHUTDOWN_REQUESTED); });
    connect(this, &CoffeeStateMachine::maintenanceRequested, this, [this]() { trigger(event::FINISHED); });
    connect(this, &CoffeeStateMachine::decalcificationRequested, this, [this]() { trigger(event::FINISHED); });
    connect(this, &CoffeeStateMachine::abortRequested, this, [this]() { trigger(event::ABORT_REQUESTED); });

    connect(m_simulation, &Simulation::coinInserted, this, [this]() { trigger(event::COIN_INSERTED); });

    connect(m_coinChecker, &CoinChecker::coinDetected, this, [this]() { trigger(event::COIN_DETECTED); });

    qDebug() << "Initial State:" << (int)current_state_;

}

CoffeeStateMachine::~CoffeeStateMachine() {
}


void CoffeeStateMachine::trigger(enum event e) {
    qDebug() << "Exit State:" << (int)current_state_;
    switch (current_state_) {
    case state::preparationStart:
        if (e == event::START_UP) {
            current_state_ = state::readySelectCoffee;
        }
        break;

    case state::readySelectCoffee:
        switch (e) {
        case event::COFFEE_SELECTED:
            current_state_ = state::confirmIntensity;
            emit this->readySelectCoffee();
            break;
        case event::SHUTDOWN_REQUESTED: current_state_ = state::performShutdown;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::confirmIntensity:
        switch (e) {
        case event::INTENSITY_CONFIRMED: current_state_ = state::waitingForCoin;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default: break;
        }
        break;

    //case state::payment:
    case state::waitingForCoin:
        switch (e) {
        case event::COIN_INSERTED: current_state_ = state::coinInserted;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default: break;
        }
        break;

    case state::coinInserted:
        if(e == event::COIN_DETECTED) current_state_ = state::checkCoin;
            break;



    case state::checkCoin:
        switch (e) {
        case event::COIN_VALID: current_state_ = state::validCoin;
            break;
        case event::COIN_INVALID: current_state_ = state::returnInvalid;
            break;
        default: break;
        }
        break;

    case state::validCoin:
        if (e == event::COIN_BOOKED) {
            current_state_ = state::bookCoin;
            emit newBalance();
        }
        break;

    case state::returnInvalid:
        switch (e) {
        case event::COIN_INVALID_RETURNED: current_state_ = state::waitingForCoin;
            break;
        // case event::COIN_STUCK: current_state_ = state::maintenance;
        //     break;
        default:
            qDebug() << "default case return Invalid";
            break;
        }
        break;



    case state::bookCoin:
        switch (e) {
        case event::NEEDED_AMOUNT_REACHED: current_state_ = state::returnChange;
            break;
        case event::NEEDED_AMOUNT_NOT_REACHED: current_state_ = state::waitingForCoin;
            break;
        case event::ERROR: current_state_ = state::maintenance;
            break;


        default: break;
        }
        break;



    case state::returnChange:
        if (e == event::PAYMENT_DONE){
            current_state_ = state::makeCoffee;
        }
        break;

    case state::makeCoffee:
        switch (e) {
        case event::PREPARE_COFFEE: current_state_ = state::grindCoffee;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::grindCoffee:
        switch (e) {
        case event::GRINDING_COMPLETET: current_state_ = state::heatingWater;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::heatingWater:
        switch (e) {
        case event::WATER_TEMPERATURE_OK: current_state_ = state::waitingForCup;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::waitingForCup:
        switch (e) {
        case event::CUP_INSERTED: current_state_ = state::brewCoffee;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::brewCoffee:
        switch (e) {
        case event::COFFEE_READY_TO_TAKEOUT: current_state_ = state::takeOutCoffee;
            break;
        case event::BREWING_COMPLETET: current_state_ = state::dispenseMilk;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default: break;
        }
        break;

    case state::dispenseMilk:
        switch (e) {
        case event::COFFEE_READY_TO_TAKEOUT: current_state_ = state::takeOutCoffee;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default:
            break;
        }
        break;

    case state::takeOutCoffee:
        switch (e) {
        case event::FINISHED: current_state_ = state::readySelectCoffee;
            break;
        case event::ABORT_REQUESTED: current_state_ = state::abort;
            break;
        default: break;
        }
        break;


    case state::maintenance:
        if (e == event::FINISHED) current_state_ = state::readySelectCoffee;
        break;

    case state::abort:
        if (e == event::FINISHED) current_state_ = state::readySelectCoffee; // TODO Refill Beans etc
        break;
    default:
        qDebug() << "Change for state not defined.";
        break;
    }

    qDebug() << "Enter State:" << (int)current_state_;
    emit stateChanged(current_state_);
}



void CoffeeStateMachine::setTouchHandler(TouchHandler* m_touchHandler){
    this->m_touchHandler = m_touchHandler;

    connect(m_touchHandler, &TouchHandler::coffeeSelected, this, [this]() { trigger(event::COFFEE_SELECTED); });
    connect(m_touchHandler, &TouchHandler::intensityConfirmed, this, [this]() { trigger(event::INTENSITY_CONFIRMED); });

}

state CoffeeStateMachine::getCurrentState() {
    return this->current_state_;
}

int CoffeeStateMachine::getCurrentAmountPaid(){
    return m_payment->getCurrentAmountPaid();
}
