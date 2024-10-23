#include "ModbusSimulationEnvironment.h"
#include "TestCase.h"

int main() {
    ModbusSimulationEnvironment simulation1;
    runTestCase1(simulation1);

    ModbusSimulationEnvironment simulation2;
    runTestCase2(simulation2);
    return 0;
}
