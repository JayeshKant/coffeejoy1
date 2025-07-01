#pragma once
#include <QObject>

class PumpControl : public QObject {
    Q_OBJECT
public:
    PumpControl(QObject *p = nullptr) : QObject(p) {}
    bool started = false;
    void startPump() { started = true; }
signals:
    void targetPressureReached();
};
