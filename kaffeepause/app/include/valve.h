#pragma once

#include <QObject>
#include <QDebug>

enum class valveState {open = 1, closed = 0};
enum class valveType {water = 0, steam = 1, milk = 2, drain = 3, supply = 4};

class Valve : public QObject {
    Q_OBJECT
public:
    Valve(valveType type, QObject* parent = nullptr);
    ~Valve();

    void reset();

    void setValveState(valveState newState);
signals:
    void valveStateChanged(valveType type, valveState newState);

public:
    valveState getValveState();

private:
    valveState currentValveState = valveState::closed;

    valveType type;

};


