#pragma once
#include <QObject>

class Pump : public QObject {
    Q_OBJECT
public:
    Pump(QObject* parent = nullptr);
    ~Pump();

    void reset();

    void setRunning(bool running);
    bool isRunning();

private:
    // TODO ID?
    bool running = false;
};
