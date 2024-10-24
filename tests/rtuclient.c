// client.c
#include <modbus.h>
#include <stdio.h>
#include <errno.h>

#define SLAVE_ID 1
#define PORT "/dev/ttyUSB0"
#define BAUDRATE 9600        // communication speed

int main(void) {
    modbus_t *ctx;
    uint16_t tab_reg[16];
    int rc;

    ctx = modbus_new_rtu(PORT, BAUDRATE, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_debug(ctx, TRUE);

    modbus_set_slave(ctx, SLAVE_ID);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    printf("Modbus RTU client connected to server...\n");

    int address = 0;
    int nRegisters = 1;
    rc = modbus_read_registers(ctx, address, nRegisters, tab_reg);
    if (rc == -1) {
        fprintf(stderr, "Failed to read registers: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    printf("Register value: %d\n", tab_reg[0]);

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}