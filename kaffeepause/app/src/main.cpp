#include "mainWindow.h"
#include "touchhandler.h"
#include "touchscreen.h"
#include "coinsupply.h"
#include "simulation.h"
#include "grinder.h"
#include "thermoblock.h"
#include "brewingunit.h"
#include "pumpcontrol.h"
#include "valve.h"
#include "pump.h"
#include "milkunit.h"
#include "splashscreen.h"
#include "payment.h"
#include "coffeeselection.h"
#include "maintenance.h"
#include "startupmanager.h"
#include "preparationshutdown.h"

/* TODO logging #include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
*/

#include <QApplication>
#include <QSettings>
#include <QCommandLineParser>
#include <QtDebug>
#include <QDir>
#include <QObject>
#include <QTimer>

using std::array;

int main(int argc, char *argv[])
{
    // Set some defaults
    QCoreApplication::setApplicationName("Kaffeepause");
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication::setOrganizationName("Berliner Hochschule für Technik");
    QCoreApplication::setOrganizationDomain("bht-berlin.de");

    qDebug() << QCoreApplication::applicationName() << " v" << QCoreApplication::applicationVersion();
    qDebug() << "Current working directory" << QDir::currentPath();

    // Parse command line options
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("Project: Kaffeepause Code for a fancy coffee machine");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    // Process config file
    // const QString configFile = parser.value("config");
    // qDebug() << "Using config file" << configFile;

    // Default values are encoded here and used if not given by config file
    // Note: the file is located using the current working directory - make sure that the file and CWD is in the right spot
    // QSettings settings(configFile, QSettings::IniFormat);

    QObject object; //for deleting Object, that are pointers. Will be destroyed, when QOject is destroyed

    // Init Objects
    CoffeeSelection m_coffeeSelection;

    CoinSensor m_insertCoinSensor;

    CoinChecker m_coinChecker(&m_insertCoinSensor);
    Pump m_pump;
    Pump m_milkPump;
    Pump m_waterPump;
    TouchScreen m_touchScreen;

    Valve m_waterValve(valveType::water);
    Valve m_steamValve(valveType::steam);
    Valve m_milkValve(valveType::milk);
    Valve m_freshWaterSupplyValve(valveType::supply);

    array<Valve*, 4> m_valves= {
        &m_waterValve,
        &m_steamValve,
        &m_milkValve,
        &m_freshWaterSupplyValve
    };

    Simulation m_simulation(&m_touchScreen,
                            &m_insertCoinSensor,
                            &m_pump,
                            &m_freshWaterSupplyValve);

    Payment m_payment(&m_simulation,
                      &m_coffeeSelection);

    m_insertCoinSensor.setSimulation(&m_simulation);

    array<CoinSensor*, 6> m_coinSensors = { // QObject can't be copied, in array it would try to make a copy in for loop -> because of that: here pointer
        new CoinSensor(coinType::EUR2),
        new CoinSensor(coinType::EUR1, &object),
        new CoinSensor(coinType::EUR05, &object),
        new CoinSensor(coinType::EUR02, &object),
        new CoinSensor(coinType::EUR01, &object),
        new CoinSensor(coinType::EUR005, &object)
    };

    array<LightSensor*, 2> m_coinLightSensors = {
        new LightSensor(&m_simulation, lightSensors::hopper, &object),
        new LightSensor(&m_simulation, lightSensors::coinSupply, &object),
    };

    array<LightSensor*, 7> m_maintenanceLightSensors = {
        new LightSensor(&m_simulation, lightSensors::cremaBeanLevel, &object),
        new LightSensor(&m_simulation, lightSensors::espressoBeanLevel, &object),
        new LightSensor(&m_simulation, lightSensors::milkLevel, &object),
        new LightSensor(&m_simulation, lightSensors::dirtyWater, &object),
        new LightSensor(&m_simulation, lightSensors::wasteDisposale, &object),
        new LightSensor(&m_simulation, lightSensors::cupInserted, &object),
        new LightSensor(&m_simulation, lightSensors::freshWaterLevel, &object)
    };

    for (CoinSensor* sensor : m_coinSensors) {
        sensor->setSimulation(&m_simulation);
    }

    for (LightSensor* coinLightSensor : m_coinLightSensors) {
        m_simulation.setCoinLightSensorInSimulation(coinLightSensor->getSensorName(), coinLightSensor);
    }

    CoinSupply m_coinSupply(&m_simulation,
                            m_coinSensors,
                            m_coinLightSensors);

    m_payment.setCoinSupply(&m_coinSupply);

    m_payment.setCoinChecker(&m_coinChecker);
    m_coinChecker.setPayment(&m_payment);


    CoffeeStateMachine m_coffeeStateMachine(&m_simulation,
                                      &m_payment,
                                      &m_coinChecker,
                                      &m_coffeeSelection,
                                      &m_coinSupply);

    TouchHandler m_touchHandler(&m_coffeeStateMachine,
                                &m_touchScreen);

    m_simulation.setTouchHandler(&m_touchHandler);
    m_coffeeSelection.setTouchHandler(&m_touchHandler);

    Thermoblock m_thermoblock(&m_simulation);

    CoffeeWaiter m_coffeeWaiter(&m_payment,
                                m_maintenanceLightSensors[5],
                                &m_thermoblock, &m_simulation,
                                &m_coffeeStateMachine,
                                &m_coffeeSelection);

    Grinder m_grinder(&m_coffeeStateMachine,
                      &m_coffeeWaiter,
                      &m_simulation);

    m_coffeeWaiter.setGrinder(&m_grinder);

    PumpControl m_pumpControl(&m_simulation,
                              &m_pump);

    m_simulation.setPumpControl(&m_pumpControl);

    BrewingUnit m_brewingUnit(&m_coffeeStateMachine,
                              &m_simulation,
                              &m_coffeeWaiter,
                              &m_pumpControl,
                              &m_waterValve);

    MilkUnit m_milkUnit(& m_thermoblock,
                        &m_coffeeStateMachine,
                        &m_simulation,
                        &m_brewingUnit,
                        &m_coffeeWaiter,
                        &m_pumpControl,
                        &m_milkPump,
                        &m_steamValve,
                        &m_milkValve);

    Maintenance m_maintenance(&m_coffeeStateMachine,
                              &m_simulation,
                              &m_pumpControl,
                              &m_waterPump,
                              &m_waterValve,
                              &m_thermoblock,
                              &m_milkValve,
                              m_maintenanceLightSensors,
                              &m_freshWaterSupplyValve, &m_milkUnit);

    m_payment.setCoffeeStateMachine(&m_coffeeStateMachine);
    m_coinChecker.setCoffeeStateMachine(&m_coffeeStateMachine);
    m_coffeeStateMachine.setTouchHandler(&m_touchHandler);

    m_maintenanceLightSensors[5]->setCoffeeStateMachine(&m_coffeeStateMachine);


    array<LightSensor*, 9> m_lightSensors = {
        m_coinLightSensors[0],
        m_coinLightSensors[1],
        m_maintenanceLightSensors[0],
        m_maintenanceLightSensors[1],
        m_maintenanceLightSensors[2],
        m_maintenanceLightSensors[3],
        m_maintenanceLightSensors[4],
        m_maintenanceLightSensors[5],
        m_maintenanceLightSensors[6]
    };

    StartUpManager m_startUpManager(&m_brewingUnit,
                                    &m_coffeeSelection,
                                    &m_coffeeStateMachine,
                                    &m_coffeeWaiter,
                                    &m_coinChecker,
                                    &m_grinder,
                                    m_lightSensors,
                                    &m_maintenance,
                                    &m_milkUnit,
                                    &m_payment,
                                    &m_pump,
                                    &m_pumpControl,
                                    &m_simulation,
                                    &m_thermoblock,
                                    &m_touchHandler,
                                    &m_touchScreen,
                                    m_valves);

    PreparationShutdown m_preparationShutdown(&m_coinChecker, m_lightSensors, &m_simulation, &m_maintenance, &m_touchHandler);

    // Init GUI

    MainWindow window(&m_simulation, &m_coffeeSelection, &m_coffeeStateMachine,
                      &m_payment, &m_touchScreen, &m_touchHandler,
                      &m_thermoblock,
                      &m_waterValve, &m_milkValve, &m_steamValve, &m_freshWaterSupplyValve);

    Splashscreen splash;
    splash.show();
    app.processEvents();

    QTimer::singleShot(2000, &window, [&](){
        splash.close();
        window.show();
    });

    // Go!
    return app.exec();
    for (CoinSensor* sensor : m_coinSensors) {
        delete sensor;
    }

    for (LightSensor* coinLightSensor : m_coinLightSensors) {
        delete coinLightSensor;
    }

}
