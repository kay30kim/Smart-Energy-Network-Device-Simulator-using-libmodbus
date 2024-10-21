#ifndef EVCHARGERSERVER_H
#define EVCHARGERSERVER_H

#include "ModbusServer.h"

class EVChargerServer : public ModbusServer {
public:
    EVChargerServer();
    void initializeRegisterData(std::ofstream& file) override;
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void processPowerData() override;
    void generatePowerData() override;
    void setupServerSimulation() override;
    std::string getSimulatorName() override;
};

#endif
