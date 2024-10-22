#include "TestCase.h"
#include "EVChargerServer.h"
#include "PVInverterServer.h"
#include "HeatPumpServer.h"
#include <iostream>
#include "ModbusServer.h"
#include "sys/wait.h"

#define TEST_CASE1_INPUT1 "ev_charger_normal.txt"
#define TEST_CASE1_INPUT2 "ev_charger_normal.txt"
#define TEST_CASE1_INPUT3 "pv_inverter_daytime.txt"
#define TEST_CASE1_OUTPUT1 "ev_charger1_output.txt"
#define TEST_CASE1_OUTPUT2 "ev_charger2_output.txt"
#define TEST_CASE1_OUTPUT3 "pv_inverter_output.txt"

void runTestCase1(ModbusSimulationEnvironment& simulation) {
    std::cout << "\nRunning Test Case 1...\n";

    ModbusServer* evCharger1 = new EVChargerServer();
    evCharger1->startServer(TEST_CASE1_INPUT1, TEST_CASE1_OUTPUT1);
    evCharger1->setupServerSimulation(TCP, "10.10.2.50", "1502");

    ModbusServer* evCharger2 = new EVChargerServer();
    evCharger2->startServer(TEST_CASE1_INPUT2, TEST_CASE1_OUTPUT2);
    evCharger2->setupServerSimulation(TCP, "127.0.0.1", "1502");

    ModbusServer* pvInverter = new PVInverterServer();
    pvInverter->startServer(TEST_CASE1_INPUT3, TEST_CASE1_OUTPUT3);
    pvInverter->setupServerSimulation(TCP, "192.168.0.1", "1502");

    simulation.addServer(evCharger1);
    simulation.addServer(evCharger2);
    simulation.addServer(pvInverter);
    
    pid_t pid = fork();
    if (pid == 0) { // child process
        std::vector<std::string> inputFiles = {
            TEST_CASE1_INPUT1,
            TEST_CASE1_INPUT2,
            TEST_CASE1_INPUT3
        };
        std::vector<std::string> outputFiles = {
            TEST_CASE1_OUTPUT1
            TEST_CASE1_OUTPUT2,
            TEST_CASE1_OUTPUT3
        };
        std::vector<int> addressRegisters = {
            EV_CHARGER_CURRENT_ADDRESS_REGISTER,
            EV_CHARGER_CURRENT_ADDRESS_REGISTER,
            PV_INVERTER_CURRENT_ADDRESS_REGISTER
        };  
        simulation.processPowerDataFromFile(inputFiles, outputFiles, addressRegisters);
        exit(0);
    } else if (pid > 0) {
        simulation.runServers();
        int status;
        waitpid(pid, &status, 0);
    } else {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
}

void runTestCase2(ModbusSimulationEnvironment& simulation) {
    // NEED TO CHANGE LIKE runTestCase2
    std::cout << "\nRunning Test Case 2...\n";

    ModbusServer* evCharger1 = new EVChargerServer();
    evCharger1->startServer("ev_charger_heavy.txt", "ev_charger1_output_heavy.txt");

    ModbusServer* evCharger2 = new EVChargerServer();
    evCharger2->startServer("ev_charger_heavy.txt", "ev_charger2_output_heavy.txt");

    ModbusServer* evCharger3 = new EVChargerServer();
    evCharger3->startServer("ev_charger_heavy.txt", "ev_charger3_output_heavy.txt");

    ModbusServer* pvInverter = new PVInverterServer();
    pvInverter->startServer("pv_inverter_night.txt", "pv_inverter_output_night.txt");

    ModbusServer* heatPump = new HeatPumpServer();
    heatPump->startServer("heat_pump_winter.txt", "heat_pump_output_winter.txt");

    simulation.addServer(evCharger1);
    simulation.addServer(evCharger2);
    simulation.addServer(evCharger3);
    simulation.addServer(pvInverter);
    simulation.addServer(heatPump);

    simulation.runServers();
}
