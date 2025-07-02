#ifndef INTERNALVIEW_H
#define INTERNALVIEW_H

#include <QWidget>

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

};

#endif // INTERNALVIEW_H
