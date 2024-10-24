#include "TestCase.h"
#include "EVChargerServer.h"
#include "PVInverterServer.h"
#include "HeatPumpServer.h"
#include <iostream>
#include "ModbusServer.h"
#include "sys/wait.h"
#include "sys/prctl.h"

#define TEST_CASE1_INPUT1 "sim_ev_test_case1_normal.txt"
#define TEST_CASE1_INPUT2 "sim_heat_test_case1_airconditioner.txt"
#define TEST_CASE1_INPUT3 "sim_pv_test_case1_normal.txt"
#define TEST_CASE1_OUTPUT1 "ev_charger_output.txt"
#define TEST_CASE1_OUTPUT2 "heatpump_output.txt"
#define TEST_CASE1_OUTPUT3 "pv_inverter_output.txt"

void runTestCase1(ModbusSimulationEnvironment& simulation) {
    std::cout << "\nRunning Test Case 1...\n";

    ModbusServer* evCharger = new EVChargerServer();
    evCharger->startServer(TEST_CASE1_INPUT1, TEST_CASE1_OUTPUT1);
    evCharger->setupServerSimulation(TCP, "10.10.2.50", "1502");

    ModbusServer* heatPump = new HeatPumpServer();
    heatPump->startServer(TEST_CASE1_INPUT2, TEST_CASE1_OUTPUT2);
    heatPump->setupServerSimulation(TCP, "127.0.0.1", "1502");

    ModbusServer* pvInverter = new PVInverterServer();
    pvInverter->startServer(TEST_CASE1_INPUT3, TEST_CASE1_OUTPUT3);
    pvInverter->setupServerSimulation(TCP, "192.168.0.54", "1502");

    simulation.addServer(evCharger);
    simulation.addServer(heatPump);
    simulation.addServer(pvInverter);
    
    pid_t pid = fork();
    if (pid == 0) { // child process
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        std::vector<std::string> inputFiles = {
            TEST_CASE1_INPUT1,
            TEST_CASE1_INPUT2,
            TEST_CASE1_INPUT3
        };
        std::vector<std::string> outputFiles = {
            TEST_CASE1_OUTPUT1,
            TEST_CASE1_OUTPUT2,
            TEST_CASE1_OUTPUT3
        };
        std::vector<int> addressRegisters = {
            EV_CHARGER_CURRENT_ADDRESS_REGISTER,
            HEAT_PUMP_CURRENT_ADDRESS_REGISTER,
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
    std::cout << "\nRunning Test Case 1...\n";

    ModbusServer* evCharger = new EVChargerServer();
    evCharger->startServer(TEST_CASE1_INPUT1, TEST_CASE1_OUTPUT1);
    evCharger->setupServerSimulation(RTU, "/dev/ttyUSB0", "1502");

    //ModbusServer* heatPump = new HeatPumpServer();
    //heatPump->startServer(TEST_CASE1_INPUT2, TEST_CASE1_OUTPUT2);
    //heatPump->setupServerSimulation(RTU, "/dev/ttyUSB0", "1502");

    //ModbusServer* pvInverter = new PVInverterServer();
    //pvInverter->startServer(TEST_CASE1_INPUT3, TEST_CASE1_OUTPUT3);
    //pvInverter->setupServerSimulation(RTU, "/dev/ttyUSB0", "1502");

    simulation.addServer(evCharger);
    //simulation.addServer(heatPump);
    //simulation.addServer(pvInverter);
    
    pid_t pid = fork();
    if (pid == 0) { // child process
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        std::vector<std::string> inputFiles = {
            TEST_CASE1_INPUT1
            //TEST_CASE1_INPUT2,
            //TEST_CASE1_INPUT3
        };
        std::vector<std::string> outputFiles = {
            TEST_CASE1_OUTPUT1,
            //TEST_CASE1_OUTPUT2,
            //TEST_CASE1_OUTPUT3
        };
        std::vector<int> addressRegisters = {
            EV_CHARGER_CURRENT_ADDRESS_REGISTER,
            //HEAT_PUMP_CURRENT_ADDRESS_REGISTER,
            //PV_INVERTER_CURRENT_ADDRESS_REGISTER
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
