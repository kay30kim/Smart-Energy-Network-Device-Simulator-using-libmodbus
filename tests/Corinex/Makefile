# Compiler and flags
CC = g++
CFLAGS = -Wall -g
INCLUDE = -I./Include -I../../src

# Directories
SRCDIR = ./src
OBJDIR = ./obj

# # Modbus library object files
# MODBUS_LIB_SRCS = modbus.lo modbus-data.lo modbus-rtu.lo modbus-tcp.lo
# MODBUS_LIB_OBJS = $(addprefix ../../src/, $(MODBUS_LIB_SRCS))

# Source files for different components
DEVICES_SRCS = EVChargerServer.cpp HeatPumpServer.cpp PVInverterServer.cpp
MODBUS_SRCS = ModbusServer.cpp ModbusSimulationEnvironment.cpp FileUpdaterSingleton.cpp
TESTCASE_SRCS = TestCase.cpp TestMain.cpp

# Recursive function to get all .cpp files from subdirectories
SRC_SUBDIRS := $(shell find $(SRCDIR) -type d)
SRCS := $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRCS:.cpp=.o))
OBJDIRS := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC_SUBDIRS))

# Final executable
TARGET = modbus_simulation

# Libraries or dependencies needed
LIBS = -L../../src/.libs -lmodbus

# Default target: build all
all: $(TARGET)

# Rule to create directories if they don't exist
$(OBJDIR):
	mkdir -p $@

$(OBJDIRS):
	mkdir -p $@

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS) -lpthread 

# Pattern rule for compiling .cpp files into .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIRS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean rule to remove object and executable files
clean:
	rm -rf $(OBJDIR)
	rm -f *output.txt

# Rebuild: clean and build again
re: clean all

.PHONY: all clean re