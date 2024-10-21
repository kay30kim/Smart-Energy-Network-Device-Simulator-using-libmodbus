#include "PVInverterServer.h"
#include <iostream>
#include <fstream>

PVInverterServer::PVInverterServer() {
    fileName = "pv_inverter_registers.txt";
}

void PVInverterServer::startServer(const std::string& inputFile, const std::string& outputFile) {
    ModbusServer::startServer(inputFile, outputFile);
    std::cout << "PV Inverter Server Running...\n";
}

void PVInverterServer::processPowerData() {
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
            updateRegister(3020 + hour, power);  // 시간대별 전력 값
            std::cout << "Updated PV Inverter register for hour: " << hour << " with power: " << power << " kW" << std::endl;
        }
    }

    file.close();
}

void PVInverterServer::initializeRegisterData(std::ofstream& file) {
    file << "3002   10.0\n";
    file << "3022   100.0\n";
}

void PVInverterServer::generatePowerData() {
    std::cout << "PV Inverter power data being generated..." << std::endl;
}

void PVInverterServer::setupServerSimulation() {
    std::cout << "PV Inverter server simulation setup..." << std::endl;
}

std::string PVInverterServer::getSimulatorName() {
    return "PV Inverter";
}
