#include "TestCase.h"
#include "EVChargerServer.h"
#include "PVInverterServer.h"
#include "HeatPumpServer.h"
#include <iostream>

void runTestCase1(ModbusSimulationEnvironment& simulation) {
    // Test Case 1: EV Charger 2개, PV Inverter 1개 (평상시 사용량)
    std::cout << "\nRunning Test Case 1...\n";

    ModbusServer* evCharger1 = new EVChargerServer();
    evCharger1->startServer("ev_charger_normal.txt", "ev_charger1_output.txt");

    ModbusServer* evCharger2 = new EVChargerServer();
    evCharger2->startServer("ev_charger_normal.txt", "ev_charger2_output.txt");

    ModbusServer* pvInverter = new PVInverterServer();
    pvInverter->startServer("pv_inverter_daytime.txt", "pv_inverter_output.txt");

    // 시뮬레이션 환경에 서버 추가
    simulation.addServer(evCharger1);
    simulation.addServer(evCharger2);
    simulation.addServer(pvInverter);
    
    // 실행
    simulation.runServers();
}

void runTestCase2(ModbusSimulationEnvironment& simulation) {
    // Test Case 2: EV Charger 3개, PV Inverter 1개, Heat Pump 1개 (많은 사용량)
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

    // 시뮬레이션 환경에 서버 추가
    simulation.addServer(evCharger1);
    simulation.addServer(evCharger2);
    simulation.addServer(evCharger3);
    simulation.addServer(pvInverter);
    simulation.addServer(heatPump);

    // 실행
    simulation.runServers();
}
