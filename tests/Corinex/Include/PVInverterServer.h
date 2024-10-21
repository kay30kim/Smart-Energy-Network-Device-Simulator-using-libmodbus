#ifndef PVINVERTERSERVER_H
#define PVINVERTERSERVER_H

#include "ModbusServer.h"

class PVInverterServer : public ModbusServer {
public:
    PVInverterServer();
    void initializeRegisterData(std::ofstream& file) override;
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void processPowerData() override;
    void generatePowerData() override;
    void setupServerSimulation() override;
    std::string getSimulatorName() override;
};

#endif
