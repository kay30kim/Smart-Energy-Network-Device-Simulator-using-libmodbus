#include "ModbusServer.h"

void ModbusServer::startServer(const std::string& inputFile, const std::string& outputFile) {
    this->inputFile = inputFile;
    this->outputFile = outputFile;
    std::cout << "Server Starting with input file: " << inputFile << " and output file: " << outputFile << "\n";
}

void ModbusServer::initializeFile() {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    file << "Register Address   Value\n";
    initializeRegisterData(file);
    file.close();
}

void ModbusServer::powerGenerationProcess() {
    std::cout << "Starting power generation for " << getSimulatorName() << std::endl;
    generatePowerData();
}

void ModbusServer::updateRegister(int address, float value) {
    std::ofstream file(outputFile, std::ios_base::app);  // append 모드로 기록
    if (!file.is_open()) {
        std::cerr << "Can't open output file: " << outputFile << std::endl;
        return;
    }
    file << "0x" << std::hex << address << "    " << std::dec << value << std::endl;
    file.close();
}

ModbusServer::~ModbusServer() {
    if (ctx) modbus_free(ctx);
}
