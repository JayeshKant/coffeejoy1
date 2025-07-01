#pragma once
#include <QObject>
class CoffeeInformation;

class CoffeeWaiter : public QObject {
    Q_OBJECT
public:
    CoffeeWaiter(QObject *p = nullptr) : QObject(p) {}
    CoffeeInformation *info = nullptr;
    CoffeeInformation* getCoffeeInformation() { return info; }
signals:
    void startBrewing();
};
