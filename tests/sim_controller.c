/*
 * Controller (Server)
 */

#include <errno.h>
#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "10.10.2.20" // Garibaldi IP
#define MODBUS_PORT 1503
#define NUM_REGISTERS 30
#define SIMULATOR "Garibaldi"

int main(void) {
    modbus_t *ctx;
    modbus_mapping_t *mb_mapping;
    int rc;
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int socket_fd;

    // Initialize Modbus TCP server
    ctx = modbus_new_tcp(SERVER_IP, MODBUS_PORT);
    if (!ctx) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    socket_fd = modbus_tcp_listen(ctx, 1);
    if (socket_fd == -1) {
        fprintf(stderr, "Failed to listen: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Register mapping setting
    mb_mapping = modbus_mapping_new(0, 0, NUM_REGISTERS, 0);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    printf("%s is activating..!\n", SIMULATOR);
    printf("Controller is running and waiting for changes...\n");

    for (;;) {
        int client_socket = modbus_tcp_accept(ctx, &socket_fd);
        if (client_socket == -1) {
            fprintf(stderr, "Error accepting connection: %s\n", modbus_strerror(errno));
            continue;
        }

        while (1) {
            rc = modbus_receive(ctx, query);
            if (rc == -1) {
                fprintf(stderr, "Failed to receive data: %s\n", modbus_strerror(errno));
                break;
            }

            // handling request and reply
            rc = modbus_reply(ctx, query, rc, mb_mapping);
            if (rc == -1) {
                fprintf(stderr, "Failed to reply: %s\n", modbus_strerror(errno));
                break;
            }
            int header_length = modbus_get_header_length(ctx);  // Set the header length for the protocol

            if (query[header_length] == 0x06) {  // MODBUS_FC_WRITE_SINGLE_REGISTER
                int address = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
                float value = (float)(MODBUS_GET_INT16_FROM_INT8(query, header_length + 4)) / 256;
                // printf("Current register value at %d: %d\n", address, mb_mapping->tab_registers[address]);
                mb_mapping->tab_registers[address] = (uint16_t)value;  // Update the register value
                // printf("Updated register %d with value: %6.1f\n", address, value);
                // TO DO LIST : Need to check how they control 
            }
            // Print updated registers
            printf("All registers\n");
            for (int i = 0; i < NUM_REGISTERS; i++) {
                if (mb_mapping->tab_registers[i] > 32768)
                    printf("Register[%d] = %d\n", i + 3000, mb_mapping->tab_registers[i] - 65536);
                else
                    printf("Register[%d] = %d\n", i + 3000, mb_mapping->tab_registers[i]);
            }
            printf("\n");
        }
        close(client_socket);
    }

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
