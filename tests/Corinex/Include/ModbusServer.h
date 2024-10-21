#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H

#include <iostream>
#include <modbus.h>
#include <fstream>

class ModbusServer {
protected:
    std::string inputFile;  // 읽을 파일
    std::string outputFile; // 업데이트할 파일
    std::string fileName;
    modbus_t* ctx;

public:
    void initializeFile();
    virtual void startServer(const std::string& inputFile, const std::string& outputFile); // configFile 인자를 추가    virtual void initializeFile();
    virtual void initializeRegisterData(std::ofstream& file) = 0;
    virtual void powerGenerationProcess();
    virtual void processPowerData() = 0;  // 각 서버의 전력 데이터를 처리하는 추상 메서드
    virtual void generatePowerData() = 0;
    virtual void setupServerSimulation() = 0;
    virtual std::string getSimulatorName() = 0;
    virtual ~ModbusServer();

protected:
    modbus_t* getContext() { return ctx; }
    void updateRegister(int address, float value);
};

#endif
