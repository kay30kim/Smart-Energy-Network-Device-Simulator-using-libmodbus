#ifndef HEATPUMPSERVER_H
#define HEATPUMPSERVER_H

#include "ModbusServer.h"

class HeatPumpServer : public ModbusServer {
public:
    HeatPumpServer();
    void initializeRegisterData(std::ofstream& file) override;
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void processPowerData() override;
    void generatePowerData() override;
    void setupServerSimulation() override;
    std::string getSimulatorName() override;
};

#endif
