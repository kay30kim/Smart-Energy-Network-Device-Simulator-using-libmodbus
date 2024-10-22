#include "FileUpdaterSingleton.h"
#include <cstdio>
#include <cstring>

FileUpdaterSingleton& FileUpdaterSingleton::getInstance() {
    static FileUpdaterSingleton instance;
    return instance;
}

FileUpdaterSingleton::FileUpdaterSingleton() {}

FileUpdaterSingleton::~FileUpdaterSingleton() {}

void FileUpdaterSingleton::updateFile(const std::string& file_name, int address, float value) {
    FILE *file = fopen(file_name.c_str(), "r+");  // read & write mode
    if (file == NULL) {
        fprintf(stderr, "[FileUpdaterSingleton] Can't open file: %s\n", file_name.c_str());
        return;
    }

    char buffer[256];
    int current_address;
    float current_value;
    int found = 0; // check whether they have the address or not => TBD: Think about the maximum registers number

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "0x%4d    %6f", &current_address, &current_value) == 2) {
            if (current_address == address) {
                fseek(file, -strlen(buffer), SEEK_CUR);
                fprintf(file, "0x%4d    %6.1f\n", address, value);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        fseek(file, 0, SEEK_END);
        fprintf(file, "%6d    %6.1f\n", address, value);
    }

    fclose(file);
}
