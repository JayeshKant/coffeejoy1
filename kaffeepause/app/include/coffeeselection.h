#pragma once

#include <vector>
#include <QObject>

#include <QDebug>

#include "coffeetypes.h"

using std::vector;

class TouchHandler;

class CoffeeSelection : public QObject {
    Q_OBJECT
public:
    CoffeeSelection(QObject* parent = nullptr);
    ~CoffeeSelection();

    void reset();

    void setTouchHandler(TouchHandler* m_touchHandler);

    vector<int> getGeneralAvailability() const;
    void setGeneralAvailability(const int generalAvailability[3]);

    virtual int getPriceInCent();
    void setPriceInCent();

    coffee getTypeCoffee();
    void setTypeCoffee(coffee typeCoffee);

    void setCoffee(coffee typeCoffee);

    intensity getCoffeeIntensity();
    void setCoffeeIntensity(intensity chosenIntensity);

    bool confirm();
    void sendConfirmation();

private:
    TouchHandler* m_touchHandler;

    int generalAvailability[3];
    int priceInCent = 0;
    coffee typeCoffee = coffee::undefined;
    intensity chosenIntensity = intensity::medium; //standard is medium
};

