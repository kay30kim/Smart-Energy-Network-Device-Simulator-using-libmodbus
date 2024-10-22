#ifndef TESTCASE_H
#define TESTCASE_H

#include "ModbusSimulationEnvironment.h"

// Test Case 1: EV Charger 2, PV Inverter 1
void runTestCase1(ModbusSimulationEnvironment& simulation);

// Test Case 2: EV Charger 3, PV Inverter 1, Heat Pump 1
void runTestCase2(ModbusSimulationEnvironment& simulation);

#endif
