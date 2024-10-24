#include "ModbusServer.h"
#include <cstring>
#include <string>

ModbusServer::ModbusServer() {
    port = NULL;
    ctx = NULL;
    ip_or_device = NULL;
    memset(query, 0, sizeof(query));
    rc = 0;
    header_length = 0;
    use_backend = -1;
}

void ModbusServer::setupServerSimulation(int use_backend, const char* ip_or_device, const char* port) {
    this->use_backend = use_backend;
    this->ip_or_device = set_ip_or_device_simulation(use_backend, ip_or_device);
    this->port = port;

    if (use_backend < TCP || use_backend > RTU || ip_or_device == NULL) {
        printf("Put right backend or IP\n");
        return ;
    }
}

// Function to set the IP address or device based on the selected backend
const char* ModbusServer::set_ip_or_device_simulation(int use_backend, const char* ip_or_device) {
    if (ip_or_device) {
        return ip_or_device;  // User-provided IP or device
    }

    // if ip_or_devices is NULL -> default setting
    switch (use_backend) {
        case TCP:
            return "127.0.0.1";
        case TCP_PI:
            return "::1";
        case RTU:
            return "/dev/ttyUSB0";
        default:
            return NULL;
    }
}

modbus_t* ModbusServer::initialize_modbus_context_simulation() {
    modbus_t* ctx = nullptr;
    if (use_backend == TCP) {
        ctx = modbus_new_tcp(ip_or_device, std::stoi(port));
    } else if (use_backend == TCP_PI) {
        ctx = modbus_new_tcp_pi(ip_or_device, port);
    } else {
        ctx = modbus_new_rtu(ip_or_device, std::stoi(port), 'N', 8, 1);
        modbus_set_slave(ctx, SERVER_ID);
    }
    return ctx;
}

// Set up the Modbus server for communication
int ModbusServer::setup_server_simulation(int use_backend, modbus_t* ctx) {
    int socket_file_descriptor = -1;
    if (use_backend == TCP) {
        socket_file_descriptor = modbus_tcp_listen(ctx, 2);
        if (socket_file_descriptor == -1) {
            fprintf(stderr, "Failed to listen: %s\n", modbus_strerror(errno));
            return -1;
        }
    } else if (use_backend == TCP_PI) {
        socket_file_descriptor = modbus_tcp_pi_listen(ctx, 1);
        modbus_tcp_pi_accept(ctx, &socket_file_descriptor);
    } else {
        int rc = modbus_connect(ctx);
        if (rc == -1) {
            fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
            modbus_free(ctx);
            return -1;
        }
    }
    return socket_file_descriptor;
}

ModbusServer::~ModbusServer() {
    if (ctx) modbus_free(ctx);
}
