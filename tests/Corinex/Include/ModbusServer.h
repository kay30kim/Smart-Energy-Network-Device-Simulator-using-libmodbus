#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H

#include <iostream>
#include <modbus.h>
#include "ModbusConstants.h"

class ModbusServer {
protected:
    modbus_t* ctx;
    const char* port;
    const char* ip_or_device;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];  // Buffer for receiving Modbus requests
    int rc;
    int header_length;
    int use_backend;

public:
    ModbusServer();
    modbus_t* initialize_modbus_context_simulation();
    void setupServerSimulation(int use_backend, const char* ip_or_device, const char* port);
    const char* set_ip_or_device_simulation(int use_backend, const char* ip_or_device);
    int setup_server_simulation(int use_backend, modbus_t* ctx);
    virtual void startServer(const std::string& inputFile, const std::string& outputFile) = 0;
    virtual void initializeInputFile(const std::string& inputFile) = 0;
    virtual void initializeOutputFile(const std::string& outputFile) = 0;
    virtual void processPowerDataFromModbusDevice() = 0;  // Modbus Connection
    virtual void setModbusRegister(int address, uint16_t power) = 0;
    virtual ~ModbusServer();

protected:
    modbus_t* getContext() { return ctx; }
};

#endif
