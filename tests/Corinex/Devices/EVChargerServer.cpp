#include "EVChargerServer.h"
#include <iostream>
#include <fstream>

EVChargerServer::EVChargerServer() {
    fileName = "ev_charger_registers.txt";
}

void EVChargerServer::startServer(const std::string& inputFile, const std::string& outputFile) {
    ModbusServer::startServer(inputFile, outputFile);
    std::cout << "EV Charger Server Running...\n";
}

void EVChargerServer::processPowerData() {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Can't open input file: " << inputFile << std::endl;
        return;
    }

    std::string line;
    int hour;
    float power;

    std::getline(file, line);  // 첫 줄은 헤더 스킵

    while (std::getline(file, line)) {
        if (sscanf(line.c_str(), "%d:%*d\t%f", &hour, &power) == 2) {
            updateRegister(3000 + hour, power);  // 시간대별 전력 값
            std::cout << "Updated EV Charger register for hour: " << hour << " with power: " << power << " kW" << std::endl;
        }
    }

    file.close();
}

void EVChargerServer::initializeRegisterData(std::ofstream& file) {
    file << "3004   10.0\n";
    file << "3024   100.0\n";
}

void EVChargerServer::generatePowerData() {
    std::cout << "EV Charger power data being generated..." << std::endl;
}

void EVChargerServer::setupServerSimulation() {
    std::cout << "EV Charger server simulation setup..." << std::endl;
    // 실제 Modbus 서버 세부 설정 코드 추가
}

std::string EVChargerServer::getSimulatorName() {
    return "EV Charger";
}
