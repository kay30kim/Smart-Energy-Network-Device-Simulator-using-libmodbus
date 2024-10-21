#ifndef TESTCASE_H
#define TESTCASE_H

#include "ModbusSimulationEnvironment.h"

// Test Case 1: EV Charger 2개, PV Inverter 1개 (평상시 사용량)
void runTestCase1(ModbusSimulationEnvironment& simulation);

// Test Case 2: EV Charger 3개, PV Inverter 1개, Heat Pump 1개 (많은 사용량)
void runTestCase2(ModbusSimulationEnvironment& simulation);

#endif // TESTCASE_H
