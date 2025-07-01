#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QThread>
#include <QProcess>
#include <QPixmap>
#include <QEventLoop>
#include <QRandomGenerator>

#include "coffeewaiter.h"
#include "coffeestatemachine.h"
#include "coffeeselection.h"
#include "payment.h"
#include "touchscreen.h"
#include "touchhandler.h"
#include "simulation.h"
#include "lightsensor.h"
#include "internalview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(
        Simulation* m_simulation,
        CoffeeSelection* m_coffeeSelection,
        CoffeeStateMachine* m_stateMachine,
        Payment* m_payment,
        TouchScreen* m_touchScreen,
        TouchHandler* m_touchHandler,
        Thermoblock* m_thermoblock,
        QWidget* parent = nullptr);

    ~MainWindow();

signals:
    void coinInserted(CoinData insertedCoin); // Payment

private:
    Ui::MainWindow *ui; // Benutzeroberfläche (generiert durch Qt Designer)
    QEventLoop loop; // Event-Schleife, um den Fortschritt zu pausieren
    coffee selectedCoffee = coffee::undefined; // Speichert den ausgewählten Kaffee (1 für Crema, 2 für Espresso, 3 für Cappuccino)
    float totalMoney = 0.0; // Gesamtbetrag des eingeworfenen Geldes
    float coffeePrice = 0.0; // Preis des ausgewählten Kaffees

    Simulation* m_simulation;
    CoffeeStateMachine* m_stateMachine;
    CoffeeSelection* m_coffeeSelection;
    Payment* m_payment;
    TouchScreen* m_touchScreen;
    TouchHandler* m_touchHandler;
    internalview* internalViewWindow = nullptr;
    Thermoblock* m_thermoblock = nullptr;

private slots:
    void buttonClicked();
    void continueProgress(); // Slot für den Fortsetzen-Button während der Fortschrittsanimation
    void onEurButtonClicked(); // Slot für die EUR-Einwurf-Buttons
    void onCupInserted();
    void onCupRemoved();
    void updateButtonsForCurrentState(state newState);
    void updateMoneyDisplay(); // Aktualisiert die Geldanzeige

    void onInternalviewClicked();
};
