#ifndef EVCHARGERSERVER_H
#define EVCHARGERSERVER_H

#include "ModbusServer.h"

class EVChargerServer : public ModbusServer {
public:
    EVChargerServer();
    ~EVChargerServer(){}
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void initializeInputFile(const std::string& inputFile) override;
    void initializeOutputFile(const std::string& outputFile) override;
    void processPowerDataFromModbusDevice() override;
};

#endif
