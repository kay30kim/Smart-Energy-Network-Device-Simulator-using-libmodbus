#include "ModbusSimulationEnvironment.h"

void ModbusSimulationEnvironment::addServer(ModbusServer* server) {
    servers.push_back(server);
}

void ModbusSimulationEnvironment::runServers() {
    for (ModbusServer* server : servers) {
        std::thread t(&ModbusServer::processPowerData, server);
        t.join();  // 각 서버의 전력 데이터를 처리하는 스레드 실행
    }
}

ModbusSimulationEnvironment::~ModbusSimulationEnvironment() {
    for (ModbusServer* server : servers) {
        delete server;
    }
}