#include "ModbusServerFactory.h"
#include "ModbusSimulationEnvironment.h"
#include "TestCase.h"

int main() {
    // // Factory를 이용하여 서버 인스턴스 생성
    // ModbusServer* evCharger = ModbusServerFactory::createServer("EVCharger");
    // ModbusServer* pvInverter = ModbusServerFactory::createServer("PVInverter");
    // ModbusServer* heatPump = ModbusServerFactory::createServer("HeatPump");

    // // Composite 패턴으로 여러 서버를 실행하는 환경 구성
    // ModbusSimulationEnvironment simulation;
    // simulation.addServer(evCharger);
    // simulation.addServer(pvInverter);
    // simulation.addServer(heatPump);

    // // 서버들 실행
    // simulation.runServers();

    // 첫 번째 테스트 케이스 실행
    ModbusSimulationEnvironment simulation1;
    runTestCase1(simulation1);

    // 두 번째 테스트 케이스 실행
    ModbusSimulationEnvironment simulation2;
    runTestCase2(simulation2);
    return 0;
}
