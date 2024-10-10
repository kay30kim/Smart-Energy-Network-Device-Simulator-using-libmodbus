/*
* Substation (Client)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <modbus.h>

#define PV_FILE "pv_inverter_registers.txt"
#define EV_FILE "ev_charger_registers.txt"
#define HP_FILE "heat_pump_registers.txt"
#define SM_FILE "substation_registers.txt"
#define PV_INVERTER 0
#define EV_CHARGER 1
#define HEAT_PUMP 1
#define NUM_REGISTERS 30    // substaion meter has 0x3000 ~ 0x3030 address

#define CONTROLLER_IP "10.10.2.20"
#define MODBUS_PORT 1503
#define SIMULATOR "Substaion Meter"

void initialize_registers(float *registers, char *filename, int im_ex_port, modbus_t *ctx);
void check_file_and_notify(float *prev_values, modbus_t *ctx, char *filename, int im_ex_port);
modbus_t* initialize_modbus(const char *ip, int port);
void initialize_file(float *prev_values);
void updateTotalValue(modbus_t *ctx, float *prev_values);
void update_file(float *prev_values);

float totalCurrent, totalVoltage;

void initialize_registers(float *registers, char *filename, int im_ex_port, modbus_t *ctx) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file: %s\n", strerror(errno));
        return;
    }

    char line[256], currentOrVoltage = 0;
    while (fgets(line, sizeof(line), file)) {
        int address;
        float value;
        if (strcmp(line, "Current Register\n") == 0) {
            currentOrVoltage = 1;
        }
        else if (strcmp(line, "Voltage Register\n") == 0) {
            currentOrVoltage = 0;
        }
        else if (sscanf(line, "0x%4d    %6f", &address, &value) == 2) {
            if (address >= 3000)
                address -= 3000;
            if (address > NUM_REGISTERS)
                printf("Error : Register address is out of range(0x3000 ~ 0x3030)");
            else {
                registers[address] = value;
                if (im_ex_port == 1) {  // import - heat pump or ev charger
                    if (currentOrVoltage)
                        totalCurrent -= value;
                    else
                        totalVoltage -= value;
                }
                else {                  // export - inverter
                    if (currentOrVoltage)
                        totalCurrent += value;
                    else
                        totalVoltage += value;
                }
                if (ctx != NULL) {
                    int rc = modbus_write_register(ctx, address, value);
                    if (rc == -1) {
                        fprintf(stderr, "Failed to notify controller: %s\n", modbus_strerror(errno));
                    } else {
                        printf("Controller notified: Register %d updated to %f\n", address, value); //Need to be commented
                    }
                }
                printf("Total current = %.1f, Total voltage = %.1f\n", totalCurrent, totalVoltage);
            }
        }
    }
    fclose(file);
}

void check_file_and_notify(float *prev_values, modbus_t *ctx, char *filename, int im_ex_port) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file: %s\n", strerror(errno));
        return;
    }

    char line[256], currentOrVoltage = 0;
    while (fgets(line, sizeof(line), file)) {
        int address;
        float value;
        if (strcmp(line, "Current Register\n") == 0)
            currentOrVoltage = 1;
        else if (strcmp(line, "Voltage Register\n") == 0)
            currentOrVoltage = 0;
        else if (sscanf(line, "0x%4d    %6f", &address, &value) == 2) { // NUM registers가 수정되어야 함
            if (address >= 3000)                         // pv Inverter 쪽에서 current랑 voltage를 적어야한다.
                address -= 3000;
            if (address > NUM_REGISTERS)
                printf("Error : Register address is out of range(0x3000 ~ 0x3030)");
            else if (prev_values[address] != value) {
                printf("%s: register %d changed from %.1f to %.1f\n", filename, address, prev_values[address], value);
                printf("currentOrVoltabe = %d, totalCurrent = %.1f\n", currentOrVoltage, totalCurrent);
                if (im_ex_port == 1) {  // import - heat pump or ev charger
                    if (currentOrVoltage)
                        totalCurrent = totalCurrent + prev_values[address] - value;
                    else
                        totalVoltage = totalVoltage + prev_values[address] - value;
                }
                else {                  // export - inverter
                    if (currentOrVoltage)
                        totalCurrent = totalCurrent - prev_values[address] + value;
                    else
                        totalVoltage = totalVoltage - prev_values[address] + value;
                }
                prev_values[address] = value;
                printf(">currentOrVoltabe = %d, totalCurrent = %.1f\n", currentOrVoltage, totalCurrent);
                if (ctx != NULL) {
                    int rc = modbus_write_register(ctx, address, value);
                    if (rc == -1) {
                        fprintf(stderr, "Failed to notify controller: %s\n", modbus_strerror(errno));
                    } else {
                        printf("Controller notified: Register %d updated to %f\n", address, value); //Need to be commented
                    }
                    updateTotalValue(ctx, prev_values);
                }
                update_file(prev_values);
                printf("Total current = %.1f, Total voltage = %.1f\n", totalCurrent, totalVoltage);
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

void update_file(float *prev_values) {
    printf("Registers print.\n");
    for (int i = 0; i <NUM_REGISTERS; i++) {
        printf("%d : %f\n", 3000+i, prev_values[i]);
    }
    FILE *file = fopen(SM_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create file\n");
        return;
    }
    fprintf(file, "Current Registers\n");
    fprintf(file, "Address Value\n");
    for (int i = 0 ; i <= 4; i += 2) {
        fprintf(file, "0x300%d    %6.1f\n", i, prev_values[i]);
    }
    fprintf(file, "[Current Avg] 0x3010 : %6.1f\n\n", totalCurrent);
    fprintf(file, "Voltage Registers\n");
    fprintf(file, "Address Value\n");
    for (int i = 0 ; i <= 4; i += 2) {
        fprintf(file, "0x302%d    %6.1f\n", i, prev_values[20+i]);
    }
    fprintf(file, "[Voltage Avg] 0x3026 : %6.1f\n", totalVoltage);
    fclose(file);
}

void initialize_file(float *prev_values) {
    // Try to open the file in read mode
    FILE *file_read = fopen(SM_FILE, "r");
    if (file_read != NULL) {
        // File exists, so we close it and do nothing
        fclose(file_read);
        return;
    }
    FILE *file = fopen(SM_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create file\n");
        return;
    }
    fprintf(file, "Current Registers\n");
    fprintf(file, "Address Value\n");
    for (int i = 0 ; i <= 4; i += 2) {
        fprintf(file, "0x300%d    %6.1f\n", i, prev_values[i]);
    }
    fprintf(file, "[Current Avg] 0x3010 : %6.1f\n\n", totalCurrent);
    fprintf(file, "Voltage Registers\n");
    fprintf(file, "Address Value\n");
    for (int i = 0 ; i <= 4; i += 2) {
        fprintf(file, "0x302%d    %6.1f\n", i, prev_values[20+i]);
    }
    fprintf(file, "[Voltage Avg] 0x3026 : %6.1f\n", totalVoltage);
    fclose(file);
}

void updateTotalValue(modbus_t *ctx, float *prev_values) {
    int rc = modbus_write_register(ctx, 10, totalCurrent);
    if (rc == -1) {
        fprintf(stderr, "Total Current update failed to notify controller: %s\n", modbus_strerror(errno));
    } else {
        printf("Controller notified: Register %d updated to %.1f\n", 3010, totalCurrent); //Need to be commented
    }
    rc = modbus_write_register(ctx, 26, totalVoltage);
    if (rc == -1) {
        fprintf(stderr, "Total Voltage update failed to notify controller: %s\n", modbus_strerror(errno));
    } else {
        printf("Controller notified: Register %d updated to %.1f\n", 3026, totalVoltage); //Need to be commented
    }
    prev_values[10] = totalCurrent;
    prev_values[26] = totalVoltage;
}

int main(int argc, char* argv[]) {
    float prev_values[NUM_REGISTERS] = {0,};

    printf("%s is activating..!\n", SIMULATOR);

    // Modbus TCP connection to the controller
    modbus_t *ctx = initialize_modbus(CONTROLLER_IP, MODBUS_PORT);

    initialize_registers(prev_values, PV_FILE, PV_INVERTER, ctx);
    initialize_registers(prev_values, EV_FILE, EV_CHARGER, ctx);
    initialize_registers(prev_values, HP_FILE, HEAT_PUMP, ctx);
    updateTotalValue(ctx, prev_values);
    initialize_file(prev_values);
    printf("Initialized registers with files from simulators and generated one file for Substation.\n\n");
    printf("Registers print.\n");
    for (int i = 0; i <NUM_REGISTERS; i++) {
        printf("%d : %f\n", 3000+i, prev_values[i]);
    }

    if (ctx == NULL) {
        return -1;
    }

    printf("Monitoring file for changes...\n");

    while (1) {
        check_file_and_notify(prev_values, ctx, PV_FILE, PV_INVERTER);
        check_file_and_notify(prev_values, ctx, EV_FILE, EV_CHARGER);
        check_file_and_notify(prev_values, ctx, HP_FILE, HEAT_PUMP);
        sleep(1);
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
