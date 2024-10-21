#include "HeatPumpServer.h"
#include <iostream>
#include <fstream>

HeatPumpServer::HeatPumpServer() {
    fileName = "heat_pump_registers.txt";
}

void HeatPumpServer::startServer(const std::string& inputFile, const std::string& outputFile) {
    ModbusServer::startServer(inputFile, outputFile);
    std::cout << "Heat Pump Server Running...\n";
}

void HeatPumpServer::processPowerData() {
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
            updateRegister(3050 + hour, power);  // 시간대별 전력 값
            std::cout << "Updated Heat Pump register for hour: " << hour << " with power: " << power << " kW" << std::endl;
        }
    }

    file.close();
}

void HeatPumpServer::initializeRegisterData(std::ofstream& file) {
    file << "3006   10.0\n";
    file << "3020   100.0\n";
}


void HeatPumpServer::generatePowerData() {
    std::cout << "Heat Pump power data being generated..." << std::endl;
}

void HeatPumpServer::setupServerSimulation() {
    std::cout << "Heat Pump server simulation setup..." << std::endl;
}

std::string HeatPumpServer::getSimulatorName() {
    return "Heat Pump";
}
