#ifndef MODBUSSIMULATIONENVIRONMENT_H
#define MODBUSSIMULATIONENVIRONMENT_H

#include <vector>
#include <thread>
#include "ModbusServer.h"

class ModbusSimulationEnvironment {
private:
    std::vector<ModbusServer*> servers;
    // std::vector<std::thread> threads; // not use now

public:
    void addServer(ModbusServer* server);
    void runServers();
    void processPowerDataFromFile(const std::vector<std::string>& inputFiles, const std::vector<std::string>& outputFiles, const std::vector<int>& addressRegisters);
    ~ModbusSimulationEnvironment();
};

#endif
