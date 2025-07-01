#include "touchscreen.h"
#include <QDebug>

TouchScreen::TouchScreen(QObject* parent) : QObject(parent) {
qDebug() << "TouchScreen: created";
}

void TouchScreen::reset(){
    qDebug("TouchScreen::reset");
    x = 0;
    y = 0;
}


void TouchScreen::setXY(int x, int y){
    qDebug() << "TouchScreen::setXY() called with x =" << x << " y =" << y;
    this->x = x;
    this->y = y;

    emit touchEvent();
}

std::pair<int, int> TouchScreen::getXY(){
    qDebug() << "TouchScreen::getXY() ->" << this->x << this->y;
    return {this->x, this->y};
}

