#include "HeatPumpServer.h"
#include "FileUpdaterSingleton.h"
#include <iostream>
#include <fstream>

HeatPumpServer::HeatPumpServer() {
    inputFile = "";
    outputFile = "";
    mb_mapping = NULL;
}

void HeatPumpServer::startServer(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "Heat Pump Server Running...\n";
    initializeInputFile(inputFile);
    initializeOutputFile(outputFile);

    // Initialize Modbus mapping for registers
    mb_mapping = modbus_mapping_new_start_address(0, 0, 0, 0, 0, NUM_REGISTERS, 0, 0);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return ;
    }

    // Initialize all registers to 10
    for (int i = 0; i < NUM_REGISTERS; i++) {
        mb_mapping->tab_registers[i] = 10;
    }
}

void HeatPumpServer::initializeInputFile(const std::string& inputFile) {
    std::cout << "Initializing input file: " << inputFile << std::endl;
    this->inputFile = inputFile;
    
}

void HeatPumpServer::initializeOutputFile(const std::string& outputFile) {
    std::cout << "Initializing output file: " << outputFile << std::endl;
    this->outputFile = outputFile;
    // Try to open the file in read mode
    FILE *file_read = fopen(outputFile.c_str(), "r");
    if (file_read != NULL) {
        // File exists, so we close it and do nothing
        fclose(file_read);
        return;
    }
    FILE *file = fopen(outputFile.c_str(), "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create file\n");
        return;
    }
    fprintf(file, "Current Register\n");
    fprintf(file, "Address Value\n");
    fprintf(file, "0x%4d    %6.1f\n\n", HEAT_PUMP_CURRENT_ADDRESS_REGISTER, 10.0);
    fprintf(file, "Voltage Register\n");
    fprintf(file, "Address Value\n");
    fprintf(file, "0x%4d    %6.1f\n", HEAT_PUMP_VOLTAGE_ADDRESS_REGISTER, 100.0);
    fclose(file);
}

void HeatPumpServer::processPowerDataFromModbusDevice() {
    int socket_file_descriptor;
    fd_set read_fds;  // fd set for select()
    int max_fd;
    struct timeval timeout;  // timeout for select()
    while (TRUE) {
        printf("Waiting for request from 1T modem...\n");
        // Initialize Modbus context
        ctx = initialize_modbus_context_simulation();
        if (ctx == NULL) {
            fprintf(stderr, "Failed to create Modbus context\n");
            return ;
        }
        // Set debug mode for logging
        modbus_set_debug(ctx, TRUE);

        // Set up the server
        socket_file_descriptor = setup_server_simulation(use_backend, ctx);
        if (socket_file_descriptor == -1) {
            printf("socket_file_descriptor = -1\n");
            modbus_mapping_free(mb_mapping);
            modbus_free(ctx);
            return ;
        }

        header_length = modbus_get_header_length(ctx);  // Set the header length for the protocol

        FD_ZERO(&read_fds);
        FD_SET(socket_file_descriptor, &read_fds);
        max_fd = socket_file_descriptor;

        timeout.tv_sec = 1000;
        timeout.tv_usec = 0;

        // Main loop for handling Modbus requests
        for (;;) {
            fd_set temp_fds = read_fds;
            int activity = select(max_fd + 1, &temp_fds, NULL, NULL, &timeout);

            if (activity < 0 && errno != EINTR) {
                fprintf(stderr, "Error in select: %s\n", modbus_strerror(errno));
                break;
            }

            if (activity == 0) {
                printf("Timeout occurred, no client requests.\n");
                continue;
            }

            // Check for new client connections
            if (FD_ISSET(socket_file_descriptor, &temp_fds)) {
                int client_socket = modbus_tcp_accept(ctx, &socket_file_descriptor);
                if (client_socket == -1) {
                    fprintf(stderr, "Error in modbus_tcp_accept: %s\n", modbus_strerror(errno));
                    continue;
                }
                printf("Client connected!\n");
                FD_SET(client_socket, &read_fds);
                if (client_socket > max_fd) {
                    max_fd = client_socket;
                }
            }

            // Handle requests from connected clients
            for (int fd = 0; fd <= max_fd; ++fd) {
                if (FD_ISSET(fd, &temp_fds) && fd != socket_file_descriptor) {
                    do {
                        rc = modbus_receive(ctx, query);
                    } while (rc == 0);  // Wait until non-zero data is received
                    if (rc == -1 && errno != EMBBADCRC) {
                        printf("Communication error: %s\n", modbus_strerror(errno));
                        close(fd);
                        FD_CLR(fd, &read_fds);
                        break;
                    }
                    printf("header_legnth = %d, query[header_length] = %d\n", header_length, query[header_length]);
                    // Handle register update requests (single register, multiple registers)
                    if (query[header_length] == 0x06) {  // MODBUS_FC_WRITE_SINGLE_REGISTER
                        int address = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
                        // TODO : the value takes the input * 256 because it is Big Endian so it takes MSB first.
                        // It need to be changed in the client side.
                        uint16_t value = (MODBUS_GET_INT16_FROM_INT8(query, header_length + 4)) / 256;
                        printf("Current register value at %d: %d\n", address, mb_mapping->tab_registers[address]);
                        mb_mapping->tab_registers[address] = value;  // Update the register value
                        printf("Updated register %d with value: %d\n", address, value);
                        // Update the file with the new register value
                        FileUpdaterSingleton::getInstance().updateFile(outputFile, address + 3000, value);
                    } else if (query[header_length] == 0x05) {  // MODBUS_FC_WRITE_SINGLE_COIL
                        int address = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
                        uint8_t value = query[header_length + 4];
                        printf("Current coil value at %d: %d\n", address, mb_mapping->tab_bits[address]);
                        mb_mapping->tab_bits[address] = value ? 1 : 0;
                        printf("Updated coil %d with value: %d\n", address, value);
                    } else if (query[header_length] == 0x10) {  // MODBUS_FC_WRITE_MULTIPLE_REGISTERS
                        int address = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
                        int reg_count = MODBUS_GET_INT16_FROM_INT8(query, header_length + 3);
                        for (int i = 0; i < reg_count; i++) {
                            float value = (float)(MODBUS_GET_INT16_FROM_INT8(query, header_length + 7 + i * 2));
                            printf("Current register value at %d: %d\n", address + i, mb_mapping->tab_registers[address + i]);
                            mb_mapping->tab_registers[address + i] = (uint16_t)value;
                            printf("Updated register %d with value: %6.1f\n", address + i, value);
                        }
                    }

                    // Send response to the Modbus client
                    rc = modbus_reply(ctx, query, rc, mb_mapping);
                    if (rc == -1) {
                        printf("Error in sending reply: %s\n", modbus_strerror(errno));
                        close(fd);
                        FD_CLR(fd, &read_fds);
                        break;
                    }
                    printf("\n");
                }
            }
        }
        // Cleanup and shutdown
        if (use_backend == TCP || use_backend == TCP_PI) {
            if (socket_file_descriptor != -1) {
                close(socket_file_descriptor);
            }
        }
        modbus_close(ctx);
        modbus_free(ctx);
        printf("\n");
    }
    modbus_mapping_free(mb_mapping);
}

void HeatPumpServer::setModbusRegister(int address, uint16_t power) {
    mb_mapping->tab_registers[address - 3000] = static_cast<uint16_t>(power);
}
