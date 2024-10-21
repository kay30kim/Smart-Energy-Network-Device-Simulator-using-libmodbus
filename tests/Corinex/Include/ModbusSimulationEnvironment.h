#ifndef MODBUSSIMULATIONENVIRONMENT_H
#define MODBUSSIMULATIONENVIRONMENT_H

#include <vector>
#include <thread>
#include "ModbusServer.h"

class ModbusSimulationEnvironment {
private:
    std::vector<ModbusServer*> servers;
    std::vector<std::thread> threads;

public:
    void addServer(ModbusServer* server);
    void runServers();
    ~ModbusSimulationEnvironment();
};

#endif
