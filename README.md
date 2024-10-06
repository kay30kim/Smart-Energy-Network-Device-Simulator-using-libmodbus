# Smart Energy Device Simulator using libmodbus

## Overview
This project provides a C-based simulator for key smart energy devices, including **PV Inverters**, **EV Chargers**, **Substation Meters**, and **Heat Pumps**, using the robust capabilities of the **libmodbus** library. This simulator establishes seamless Modbus protocol-based communication across these simulated devices, facilitating interaction between devices and connecting with an external 1T modem (metering machine) for real-time data exchange and control in a networked environment.

This simulator is designed for high scalability and performance, capable of modeling complex communication scenarios in smart energy grids, making it ideal for testing, development, and demonstration purposes in energy and automation systems.

## Key Features
- **Device Simulation**: Provides full-featured simulations for PV inverters, EV chargers, substation meters, and heat pumps.
- **Modbus Protocol**: Leverages the libmodbus library to enable seamless communication over Modbus TCP/RTU.
- **1T Modem Integration**: Connects simulated devices to an external 1T modem, simulating real-world metering and control device integration.
- **Modular and Extensible**: Built with C for high performance, and designed to support the addition of more device types and communication protocols.

## Requirements
You will need the following to compile and run this project:

- **C Compiler** (GCC or Clang)
- **libmodbus** library
- **GNU Make**
- Optional: **Automake**, **Autoconf**, **Libtool** for configuration
- For documentation: **asciidoc** and **xmlto** (optional)

## Installation
To install the project, follow these steps:

```bash
# Clone the repository
git clone https://github.com/kay30kim/Smart-Energy-Network-Device-Simulator-using-libmodbus.git
cd Smart-Energy-Network-Device-Simulator-using-libmodbus

# Compile with libmodbus
./configure
make
sudo make install

Ensure that your system’s library path is updated, which can be done by running `sudo ldconfig`.

## Usage
Run the simulator with the following command in separate terminal windows to test device communication:

```bash
# Start the PV Inverter simulation
./sim_pv_inverter

# Start the EV Charger simulation
./sim_ev_charger

# Start the Substation Meter simulation
./sim_substation_meter

# Start the Heat Pump simulation
./sim_heat_pump

# Start the 1T Modem integration
./sim_1t_modem

The simulator supports both TCP and RTU Modbus modes. Use `--help` with each simulation command for options and configurations.

## Testing
Testing is available to verify correct functionality:

```bash
make test
Run specific tests for individual devices or for the entire suite. Detailed testing information can be found in the `tests/` directory.

## Contributing
Contributions are welcome. Please refer to the `CONTRIBUTING.md` file for guidelines.

## Documentation
Comprehensive documentation can be served locally:

```bash
pip install mkdocs-material
mkdocs serve

For further information on the Modbus protocol and this project, visit the official Modbus website.