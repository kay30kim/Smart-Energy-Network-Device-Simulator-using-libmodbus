#ifndef MODBUSSERVERFACTORY_H
#define MODBUSSERVERFACTORY_H

#include "EVChargerServer.h"
#include "PVInverterServer.h"
#include "HeatPumpServer.h"

class ModbusServerFactory {
public:
    static ModbusServer* createServer(const std::string& serverType);
};

#endif
