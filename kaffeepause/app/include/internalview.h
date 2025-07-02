#ifndef INTERNALVIEW_H
#define INTERNALVIEW_H

#include <QWidget>
#include "valve.h"
namespace Ui {
class internalview;
}

class internalview : public QWidget
{
    Q_OBJECT

public:
    explicit internalview(QWidget *parent = nullptr);
    ~internalview();

private:
    Ui::internalview *ui;

public slots:
    void updateTemperatureVisual(int temperature);  // 🔥
    void updateGrindingVisual(int current , int total); // grinding visual
    void updateMilkTemperatureVisual(int temperature);
    void updatePressureVisual(int pressure);
    void updateValveVisual (valveType type , valveState state);

};

#endif // INTERNALVIEW_H
