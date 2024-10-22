#ifndef FILEUPDATERSINGLETON_H
#define FILEUPDATERSINGLETON_H

#include <string>

class FileUpdaterSingleton {
public:
    static FileUpdaterSingleton& getInstance();

    void updateFile(const std::string& file_name, int address, float value);

private:
    FileUpdaterSingleton();
    ~FileUpdaterSingleton();

    // Copy constructor and assignment operator are deleted to prevent copying.
    FileUpdaterSingleton(const FileUpdaterSingleton&) = delete;
    void operator=(const FileUpdaterSingleton&) = delete;
};

#endif 