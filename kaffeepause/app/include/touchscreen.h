#pragma once
#include <QObject>

class TouchScreen : public QObject
{
    Q_OBJECT
public:
    TouchScreen(QObject* parent = nullptr);

    void reset();

    void setXY(int x, int y);
    std::pair<int, int> getXY();

signals:
    void touchEvent();


private:
    int x = 0;
    int y = 0;

};
