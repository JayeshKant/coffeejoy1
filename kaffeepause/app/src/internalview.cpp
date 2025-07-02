#include "internalview.h"
#include "ui_internalview.h"
#include "thermoblock.h"

internalview::internalview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::internalview)
{
    ui->setupUi(this);
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
