#ifndef MODBUS_CONSTANTS_H
#define MODBUS_CONSTANTS_H

#define HEAT_PUMP_CURRENT_ADDRESS_REGISTER      3000
#define PV_INVERTER_CURRENT_ADDRESS_REGISTER    3002
#define EV_CHARGER_CURRENT_ADDRESS_REGISTER     3004
#define HEAT_PUMP_VOLTAGE_ADDRESS_REGISTER      3020
#define PV_INVERTER_VOLTAGE_ADDRESS_REGISTER    3022
#define EV_CHARGER_VOLTAGE_ADDRESS_REGISTER     3024
#define NUM_REGISTERS   30
#define SERVER_ID 1

enum {
    TCP,
    TCP_PI,
    RTU
};

#endif