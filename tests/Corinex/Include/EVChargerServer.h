#ifndef EVCHARGERSERVER_H
#define EVCHARGERSERVER_H

#include "ModbusServer.h"

class EVChargerServer : public ModbusServer {
private:
    std::string inputFile;
    std::string outputFile;
    modbus_mapping_t* mb_mapping;
public:
    EVChargerServer();
    ~EVChargerServer(){}
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void initializeInputFile(const std::string& inputFile) override;
    void initializeOutputFile(const std::string& outputFile) override;
    void processPowerDataFromModbusDevice() override;
    void setModbusRegister(int address, uint16_t power) override;
};

#endif
