#ifndef HEATPUMPSERVER_H
#define HEATPUMPSERVER_H

#include "ModbusServer.h"

class HeatPumpServer : public ModbusServer {
private:
    std::string inputFile;
    std::string outputFile;
    modbus_mapping_t* mb_mapping;
public:
    HeatPumpServer();
    ~HeatPumpServer(){}
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void initializeInputFile(const std::string& inputFile) override;
    void initializeOutputFile(const std::string& outputFile) override;
    void processPowerDataFromModbusDevice() override;
};

#endif
