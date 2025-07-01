#pragma once
#include <QObject>
#include "touchscreen.h"
#include "coffeestatemachine.h"
#include "buttonTypes.h"

#include <vector>



using std::vector;
using std::map;



class TouchHandler : public QObject
{
    Q_OBJECT
public:
    TouchHandler(CoffeeStateMachine* m_stateMachine,
                 TouchScreen* m_touchScreen,
                 QObject* parent = nullptr);
    ~TouchHandler();

    void reset();

    bool checkTouchEvent(int x, int y, Buttons button);
    void setCurrentButton(buttons button);
    buttons getCurrentButton();
    vector<Buttons> getKnownButtons();


signals:
    void buttonPressed();
    void coffeeSelected(coffee typeCoffee);
    void intensityConfirmed(intensity chosenIntensity);
    void startUp();

private slots:
    void onTouchEvent();
    void onButtonPressed();


private:

    CoffeeStateMachine* m_stateMachine;
    TouchScreen* m_touchScreen;

    state currentState;

    buttons pressedButton;

    static const map<state, vector<Buttons>> knownButtons;



};

