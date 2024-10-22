#ifndef PVINVERTERSERVER_H
#define PVINVERTERSERVER_H

#include "ModbusServer.h"

class PVInverterServer : public ModbusServer {
public:
    PVInverterServer();
    ~PVInverterServer(){}
    void startServer(const std::string& inputFile, const std::string& outputFile) override;
    void initializeInputFile(const std::string& inputFile) override;
    void initializeOutputFile(const std::string& outputFile) override;
    void processPowerDataFromModbusDevice() override;
};

#endif
