# Compiler and flags
CC = g++
CFLAGS = -Wall -g
INCLUDE = -I./Include -I../../src

# Directories
DEVICES_SRCDIR = ./Devices
MODBUS_SRCDIR = ./Modbus
TESTCASE_SRCDIR = ./TestCase
OBJDIR = ./obj
BINDIR = ./bin

# # Modbus library object files
# MODBUS_LIB_SRCS = modbus.lo modbus-data.lo modbus-rtu.lo modbus-tcp.lo
# MODBUS_LIB_OBJS = $(addprefix ../../src/, $(MODBUS_LIB_SRCS))

# Source files for different components
DEVICES_SRCS = EVChargerServer.cpp HeatPumpServer.cpp PVInverterServer.cpp
MODBUS_SRCS = ModbusServer.cpp ModbusSimulationEnvironment.cpp FileUpdaterSingleton.cpp
TESTCASE_SRCS = TestCase.cpp TestMain.cpp

# Add prefixes to source files and create object file list
DEVICES_OBJS = $(addprefix $(OBJDIR)/, $(DEVICES_SRCS:.cpp=.o))
MODBUS_OBJS = $(addprefix $(OBJDIR)/, $(MODBUS_SRCS:.cpp=.o))
TESTCASE_OBJS = $(addprefix $(OBJDIR)/, $(TESTCASE_SRCS:.cpp=.o))
OBJS = $(DEVICES_OBJS) $(MODBUS_OBJS) $(TESTCASE_OBJS)

# Final executable
TARGET = $(BINDIR)/modbus_simulation

# Libraries or dependencies needed
LIBS = -L../../src/.libs -lmodbus

# Default target: build all
all: $(TARGET)

# Rule to create directories if they don't exist
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Rule to build the target executable
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS) -lpthread 

# Pattern rule for compiling .cpp files into .o files
$(OBJDIR)/%.o: $(DEVICES_SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/%.o: $(MODBUS_SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)/%.o: $(TESTCASE_SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean rule to remove object and executable files
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	rm -f *output.txt

# Rebuild: clean and build again
re: clean all

.PHONY: all clean re