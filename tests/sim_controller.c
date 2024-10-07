/*
 * Controller (Server)
 */

#include <errno.h>
#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "10.10.1.20" // Garibaldi IP
#define MODBUS_PORT 1502
#define NUM_REGISTERS 11

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

    printf("Controller running and waiting for changes...\n");

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

            // Print updated registers
            printf("All registers\n");
            for (int i = 0; i < NUM_REGISTERS; i++) {
                printf("Register[%d] = %d\n", i, mb_mapping->tab_registers[i]);
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
