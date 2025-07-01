#include "mainWindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(
    Simulation* m_simulation,
    CoffeeSelection* m_coffeeSelection,
    CoffeeStateMachine* m_stateMachine,
    Payment* m_payment,
    TouchScreen* m_touchScreen,
    TouchHandler* m_touchHandler,
    Thermoblock* m_thermoblock,
    QWidget* parent)

        : QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_simulation(m_simulation),
        m_stateMachine(m_stateMachine),
        m_coffeeSelection(m_coffeeSelection),
        m_payment(m_payment),
        m_touchScreen(m_touchScreen),
        m_touchHandler(m_touchHandler),
        m_thermoblock(m_thermoblock){

    qDebug() << "Initilaize MainWindow";

    ui->setupUi(this);
    const QSize targetSize(1291, 1064);
    setFixedSize(targetSize);
     // Initialisierung der GUI
    // ui->removeCup->setEnabled(false); // Remove-Button deaktiviert
    connect(ui->internalview, &QPushButton::clicked,
            this, &MainWindow::onInternalviewClicked);
    qDebug() << "before connections with Euro and Cup buttons";

    // Verbindung der EUR-Buttons mit dem gemeinsamen Slot
    connect(ui->EUR_005, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked); // in Anwendungslogik -> Touchscreen funktion (const static ) position crema button attribut ->public const static
    connect(ui->EUR_01, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->EUR_02, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->EUR_05, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->EUR_1, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->EUR_2, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->Knopf, &QPushButton::clicked, this, &MainWindow::onEurButtonClicked);
    connect(ui->insertCup, &QPushButton::clicked, this, &MainWindow::onCupInserted);
    connect(ui->removeCup, &QPushButton::clicked, this, &MainWindow::onCupRemoved);

    qDebug() << "connections with Euro and Cup buttons";

    // Connections of the buttons with buttonClicked
    connect(ui->cremaButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->espressoButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->cappuccinoButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->startMachine, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->strongButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->mediumButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->lightButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->abort, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->maintenance, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->shutDown, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->decalcification, &QPushButton::clicked, this, &MainWindow::buttonClicked);


    // Show Buttons depending on State
    connect(m_stateMachine, &CoffeeStateMachine::stateChanged, this, &MainWindow::updateButtonsForCurrentState);

    // Update MoneyDisplay TODO with Jana P: I set a continous timer to update the amount should I/we change it?
    connect(m_stateMachine, &CoffeeStateMachine::newBalance, this, &MainWindow::updateMoneyDisplay);
    connect(m_payment, &Payment::newBalance, this, &MainWindow::updateMoneyDisplay);


    // Deactivate all Buttons
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i]->setEnabled(false);
    }

    ui->startMachine->setEnabled(true);

    ui->coffeeCup->setVisible(false);

    // QTimer* updateMoneyDisplayTimer = new QTimer(this);
    // connect(updateMoneyDisplayTimer, &QTimer::timeout, this, &MainWindow::updateMoneyDisplay);
    // updateMoneyDisplayTimer->start(100);

    qDebug() << "MainWindow initialized";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buttonClicked(){

    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    buttons clickedButtonType;

    if (clickedButton == ui->cremaButton) {
        clickedButtonType = buttons::crema;
    } else if (clickedButton == ui->espressoButton) {
        clickedButtonType = buttons::espresso;
    } else if (clickedButton == ui->cappuccinoButton) {
        clickedButtonType = buttons::cappuccino;
    } else if (clickedButton == ui->startMachine) {
        clickedButtonType = buttons::start;
    } else if (clickedButton == ui->maintenance) {
        clickedButtonType = buttons::maintenance;
    } else if (clickedButton == ui->shutDown) {
        clickedButtonType = buttons::shutdown;
    } else if (clickedButton == ui->decalcification) {
        clickedButtonType = buttons::decalcification;
    } else if (clickedButton == ui->abort) {
        clickedButtonType = buttons::abort;
    } else if (clickedButton == ui->strongButton) {
        clickedButtonType = buttons::strong;
    } else if (clickedButton == ui->mediumButton) {
        clickedButtonType = buttons::medium;
    } else if (clickedButton == ui->lightButton) {
        clickedButtonType = buttons::light;
    } else {
        clickedButtonType = buttons::undefined;
    }

    m_simulation->onButtonTouched(clickedButtonType);

}


void MainWindow::onCupInserted(){
    ui->coffeeCup->setVisible(true);
    m_simulation->cupInserted(true);
}

void MainWindow::onCupRemoved(){
    ui->coffeeCup->setVisible(false);
    m_simulation->cupInserted(false);
}


void MainWindow::continueProgress()
{
    loop.quit(); // Event-Schleife verlassen, um den Fortschritt fortzusetzen
}


void MainWindow::onEurButtonClicked()
{
    QPushButton* EurButton = qobject_cast<QPushButton*>(sender());

    //characteristics of the coins, diameter in micrometer, width in micrometer, weight in mg, magnetic characteristics
    static const map<QPushButton*, coinType > uiToType {
        {  ui->EUR_005,  coinType::EUR005 },
        {  ui->EUR_01,   coinType::EUR01},
        {  ui->EUR_02,   coinType::EUR02 },
        {  ui->EUR_05,   coinType::EUR05 },
        {  ui->EUR_1,    coinType::EUR1 },
        {  ui->EUR_2,    coinType::EUR2 },
        {  ui->Knopf,    coinType::INVALID }
    };

    auto it = uiToType.find(EurButton);

    if (it != uiToType.end()){
        m_simulation->onEurButtonClicked(it->second);
    }

}

void MainWindow::updateMoneyDisplay()
{
    //->>>> needs not to be payment!!! or payment needs to be in here to!qDebug() << ->getCheckedInsertedCoinValue();
    ui->displayMoney->display(m_stateMachine->getCurrentAmountPaid()); // Geldanzeige aktualisieren
}


void MainWindow::updateButtonsForCurrentState(state newState) {

    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i]->setEnabled(false);
    }
    ui->internalview->setEnabled(true);

    switch(newState) {
        case state::preparationStart:
            ui->startMachine->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::readySelectCoffee:
            ui->cremaButton->setEnabled(true);
            ui->cappuccinoButton->setEnabled(true);
            ui->espressoButton->setEnabled(true);
            ui->internalview->setEnabled(true);
            ui->abort->setEnabled(true);
            break;
        case state::confirmIntensity:
            ui->strongButton->setEnabled(true);
            ui->mediumButton->setEnabled(true);
            ui->lightButton->setEnabled(true);
            ui->startMachine->setEnabled(true);
            ui->abort->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::waitingForCoin:
            ui->EUR_005->setEnabled(true);
            ui->EUR_01->setEnabled(true);
            ui->EUR_02->setEnabled(true);
            ui->EUR_05->setEnabled(true);
            ui->EUR_1->setEnabled(true);
            ui->EUR_2->setEnabled(true);
            ui->Knopf->setEnabled(true);
            ui->abort->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::makeCoffee:
            ui->abort->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::waitingForCup:
            ui->insertCup->setEnabled(true);
            ui->abort->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::brewCoffee:
            ui->abort->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::takeOutCoffee:
            ui->removeCup->setEnabled(true);
            ui->internalview->setEnabled(true);
        case state::maintenance:
            ui->decalcification->setEnabled(true);
            ui->shutDown->setEnabled(true);
            ui->internalview->setEnabled(true);
            break;
        case state::abort:
            ui->internalview->setEnabled(true);
            // TODO Buttons for abort
            break;
        default:
            ui->internalview->setEnabled(true);
            qDebug() << "no Buttons to press";
            break;
    }
}


void MainWindow::onInternalviewClicked()
{
    if (!internalViewWindow) {
        internalViewWindow = new internalview();

        connect(m_thermoblock, &Thermoblock::temperatureChanged,
                internalViewWindow, &internalview::updateTemperatureVisual);

        connect(m_simulation, &Simulation::grindingProgress,
                internalViewWindow, &internalview::updateGrindingVisual);

        internalViewWindow->setAttribute(Qt::WA_DeleteOnClose);
    }

    internalViewWindow->show();
    internalViewWindow->raise();
    internalViewWindow->activateWindow();
    internalViewWindow->updateTemperatureVisual(
        m_thermoblock->getTargetTemperature());
}



