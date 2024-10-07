/*
* Substation (Client)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <modbus.h>

#define FILE_NAME "modbus_registers.txt"
#define NUM_REGISTERS 11

#define CONTROLLER_IP "10.10.1.20"
#define MODBUS_PORT 1502

void initialize_registers(int *registers);
void check_file_and_notify(int *prev_values, modbus_t *ctx);
modbus_t* initialize_modbus(const char *ip, int port);

void initialize_registers(int *registers) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        registers[i] = 10; // Initial value
    }
}

void check_file_and_notify(int *prev_values, modbus_t *ctx) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file: %s\n", strerror(errno));
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int address, value;
        if (sscanf(line, "%d %d", &address, &value) == 2) {
            if (address >= 0 && address < NUM_REGISTERS) {
                if (prev_values[address] != value) {
                    printf("Register %d changed from %d to %d\n", address, prev_values[address], value);
                    prev_values[address] = value;

                    if (ctx != NULL) {
                        int rc = modbus_write_register(ctx, address, value);
                        if (rc == -1) {
                            fprintf(stderr, "Failed to notify controller: %s\n", modbus_strerror(errno));
                        } else {
                            printf("Controller notified: Register %d updated to %d\n", address, value);
                        }
                    }
                }
            }
        }
    }
    fclose(file);
}

modbus_t* initialize_modbus(const char *ip, int port) {
    modbus_t *ctx = modbus_new_tcp(ip, port);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection to controller failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return NULL;
    }
    printf("Connected to controller at %s:%d\n", ip, port);
    return ctx;
}

int main(int argc, char* argv[]) {
    int prev_values[NUM_REGISTERS];
    initialize_registers(prev_values);

    // Modbus TCP connection to the controller
    modbus_t *ctx = initialize_modbus(CONTROLLER_IP, MODBUS_PORT);
    if (ctx == NULL) {
        return -1;
    }

    printf("Monitoring file for changes...\n");

    while (1) {
        check_file_and_notify(prev_values, ctx);
        sleep(1);
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
