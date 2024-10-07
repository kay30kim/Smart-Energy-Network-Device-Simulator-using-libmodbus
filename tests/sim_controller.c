/*
 * Controller
 */
#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define CONTROLLER_IP "10.10.1.20"
#define MODBUS_PORT 1502
#define NUM_REGISTERS 11

int main(void) {
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int header_length;

    // Initialize Modbus TCP server
    ctx = modbus_new_tcp(CONTROLLER_IP, MODBUS_PORT);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    printf("Controller running and waiting for changes...\n");

    mb_mapping = modbus_mapping_new(0, 0, NUM_REGISTERS, 0);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    header_length = modbus_get_header_length(ctx);

    while (1) {
        rc = modbus_receive(ctx, query);
        if (rc == -1) {
            fprintf(stderr, "Failed to receive data: %s\n", modbus_strerror(errno));
            break;
        }

        // Process the request
        rc = modbus_reply(ctx, query, rc, mb_mapping);
        if (rc == -1) {
            fprintf(stderr, "Failed to reply: %s\n", modbus_strerror(errno));
            break;
        }

        // Print updated registers
        for (int i = 0; i < NUM_REGISTERS; i++) {
            printf("Register[%d] = %d\n", i, mb_mapping->tab_registers[i]);
        }
        printf("\n");
    }

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
