#include "internalview.h"
#include "ui_internalview.h"
#include <QLabel>
internalview::internalview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::internalview)
{
    ui->setupUi(this);
    const QSize targetSize(1591, 1100);
    setFixedSize(targetSize);
}

internalview::~internalview()
{
    delete ui;
}

void internalview::updateTemperatureVisual(int temperature)
{
    ui->tempValue->setText(QString::number(0) + " °C");
    QString imagePath;

    if (temperature < 21)
        imagePath = ":/0-removebg-preview.png";
    else if (temperature < 60)
        imagePath = ":/25-removebg-preview.png";
    else if (temperature < 55)
        imagePath = ":/30-removebg-preview.png";
    else if (temperature < 65)
        imagePath = ":/35-removebg-preview.png";
    else if (temperature < 70)
        imagePath = ":/40-removebg-preview.png";
    else if (temperature < 75)
        imagePath = ":/45-removebg-preview.png";
    else if (temperature < 80)
        imagePath = ":/50-removebg-preview.png";
    else if (temperature < 85)
        imagePath = ":/55-removebg-preview.png";
    else
        imagePath = ":/60-removebg-preview.png";

    QPixmap pix(imagePath);
    ui->temperature->setPixmap(pix.scaled(ui->temperature->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->tempValue->setText(QString::number(temperature) + " °C");

}

void internalview::updateGrindingVisual(int current , int total)
{


    int percentage = current * 100 / total;
    QString path;
    if (percentage < 25)      path = ":/grinder1.png";
    else if (percentage < 50) path = ":/grinder2.png";
    else if (percentage < 75) path = ":/grinder3.png";
    else                      path = ":/grinder4.png";
    QPixmap pix(path);
    ui->grinder->setPixmap(pix.scaled(ui->grinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QString powderpath;
    if (percentage < 25)      powderpath = ":/powderpipe1.png";
    else if (percentage < 50) powderpath = ":/powderpipe2.png";
    else if (percentage < 75) powderpath = ":/powderpipe3.png";
    else                      powderpath = ":/powderpipe4.png";
    QPixmap pix1(powderpath);
    ui->powderpipe->setPixmap(pix1.scaled(ui->grinder->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

void internalview::updateMilkTemperatureVisual(int temperature)
{

    QString imagePath;
    if (temperature < 0)
        imagePath = ":/0-removebg-preview.png";
    else if (temperature < 5)
        imagePath = ":/25-removebg-preview.png";
    else if (temperature < 6)
        imagePath = ":/30-removebg-preview.png";
    else if (temperature < 10)
        imagePath = ":/35-removebg-preview.png";
    else if (temperature < 15)
        imagePath = ":/40-removebg-preview.png";
    else if (temperature < 20)
        imagePath = ":/45-removebg-preview.png";
    else if (temperature < 25)
        imagePath = ":/50-removebg-preview.png";
    else if (temperature < 30)
        imagePath = ":/55-removebg-preview.png";
    else
        imagePath = ":/60-removebg-preview.png";

    QPixmap pix(imagePath);
    ui->milktempthermo->setPixmap(pix.scaled(ui->milktempthermo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->milkTempValue->setText(QString::number(temperature)+ "°C");
}

void internalview::updatePressureVisual(int pressure)
{

    QString imagePath;
    if (pressure < 1000)
        imagePath = ":/water0.png";
    else if (pressure < 2000)
        imagePath = ":/small1.png";
    else if (pressure < 3000)
        imagePath = ":/small2.png";
    else if (pressure < 4000)
        imagePath = ":/small2";
    else
        imagePath = ":/small2";


    QString imagePath1;
    if (pressure < 4000)
        imagePath1 = ":/bentpipe1";
    else if (pressure < 5000)
        imagePath1 = ":/bentpipe3";
    else if (pressure < 6000)
        imagePath1 = ":/bentpipe3";
    else
        imagePath1 = ":/bentpipe3";

    QPixmap pix1(imagePath);
    ui->straightpipe->setPixmap(pix1.scaled(ui->straightpipe->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QPixmap pix(imagePath1);
    ui->bentpipe->setPixmap(pix.scaled(ui->bentpipe->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->pressureProgress->setText(QString::number(pressure)+ "bar");


}

void internalview::updateValveVisual(valveType type, valveState state)
{
    QLabel* valvelabel = nullptr;
    switch(type) {
    case valveType::water:
        valvelabel = ui->WaterValve;
        break;
    case valveType::milk:
        valvelabel = ui->Milkvalve;
        break;
    default:
        break;
    }
    if (!valvelabel)
        return;

    QString path;
    if (state == valveState::open)
        path = ":/valve2.png";
    else
        path = ":/valve1.png";

    QPixmap pix(path);
    valvelabel->setPixmap(pix.scaled(valvelabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void internalview::updateWaterProgress(int dispensed , int total)
{
    QString path;
    QString imagePath;
    if (dispensed < 5)
        imagePath = ":/coffefinal0.png";
    else if (dispensed < 25)
        imagePath = ":/coffeefinal1.png";
    else if (dispensed < 50)
        imagePath = ":/coffeefinal2.png";
    else if (dispensed < 100)
        imagePath = ":/coffeefinal3.png";
    else if (dispensed < 125)
        imagePath = ":/coffeefinal4.png";
    else if (dispensed < 160)
        imagePath = ":/coffeefinal5.png";
    else
        imagePath = ":/coffeefinal5";

    QPixmap pix(path);
    ui->coffeefinal->setPixmap(pix.scaled(ui->coffeefinal->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        ui->WaterLevel->setText(QString::number(dispensed)+ "/" + QString::number(total)+ "ml");
}

void internalview::updateMilkProgress(int dispensed, int total)
{
    QString path;
    QString imagePath;
    if (dispensed < 9)
        imagePath = ":/milkfinal0.png";
    else if (dispensed < 20)
        imagePath = ":/milkfinal1.png";
    else if (dispensed < 30)
        imagePath = ":/milkfinal2.png";
    else if (dispensed < 40)
        imagePath = ":/milkfinal3.png";
    else if (dispensed < 50)
        imagePath = ":/milkfinal4.png";
    else if (dispensed < 60)
        imagePath = ":/milkfinal5.png";
    else
        imagePath = ":/milkfinal5.png";

    QPixmap pix(path);
    ui->milkfinal->setPixmap(pix.scaled(ui->milkfinal->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->milklvl->setText(QString::number(dispensed) + "/" + QString::number(total) + " ml");
}
