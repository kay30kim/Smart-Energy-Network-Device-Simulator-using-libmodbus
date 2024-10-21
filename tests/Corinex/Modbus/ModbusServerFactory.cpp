#include "ModbusServerFactory.h"

ModbusServer* ModbusServerFactory::createServer(const std::string& serverType) {
    if (serverType == "EVCharger") {
        return new EVChargerServer();
    } else if (serverType == "PVInverter") {
        return new PVInverterServer();
    } else if (serverType == "HeatPump") {
        return new HeatPumpServer();
    } else {
        return nullptr;
    }
}
