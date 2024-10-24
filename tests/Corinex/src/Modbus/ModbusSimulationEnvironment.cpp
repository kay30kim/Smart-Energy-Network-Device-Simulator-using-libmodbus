#include "ModbusSimulationEnvironment.h"
#include "FileUpdaterSingleton.h"
#include <algorithm>
#include <cstring>

void ModbusSimulationEnvironment::addServer(ModbusServer* server) {
    servers.push_back(server);
}

void ModbusSimulationEnvironment::runServers() {
    for (ModbusServer* server : servers) {
        threads.emplace_back(&ModbusServer::processPowerDataFromModbusDevice, server);
    }

    // wait until all threads finish
    for (std::thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ModbusSimulationEnvironment::processPowerDataFromFile(const std::vector<std::string>& inputFiles, const std::vector<std::string>& outputFiles, const std::vector<int>& addressRegisters) {
    std::vector<FILE*> files;
    std::vector<char> buffers(inputFiles.size() * 256);  // buffer for each files
    int hour = 0;
    float power = 0.0;

    // open each files
    for (const auto& fileName : inputFiles) {
        FILE* file = fopen(fileName.c_str(), "r");
        if (file == NULL) {
            fprintf(stderr, "[ModbusSimulationEnvironment] Can't open power data file: %s\n", fileName.c_str());
            exit(1);
        }
        files.push_back(file);
    }

    // read same line in each files
    while (true) {
        for (size_t i = 0; i < files.size(); ++i) {
            if (fgets(&buffers[i * 256], 256, files[i])) {
                if (sscanf(&buffers[i * 256], "%d:00    %f", &hour, &power) == 2) {
                    servers[i]->setModbusRegister(addressRegisters[i] - 3000, power);
                    FileUpdaterSingleton::getInstance().updateFile(outputFiles[i], addressRegisters[i], power);
                    printf("Time: %2d:00 - File: %s - Generated Power: %.1f\n", hour, inputFiles[i].c_str(), power);
                }
            } else {
                // if it reads all, close it
                fclose(files[i]);
                files[i] = nullptr;
            }
        }

        // If every files is NULL, finish it.
        if (std::all_of(files.begin(), files.end(), [](FILE* f) { return f == nullptr; })) {
            break;
        }
        sleep(5);
    }

    // close every files
    for (FILE* file : files) {
        if (file != nullptr) {
            fclose(file);
        }
    }
}

ModbusSimulationEnvironment::~ModbusSimulationEnvironment() {
    for (ModbusServer* server : servers) {
        delete server;
    }
}