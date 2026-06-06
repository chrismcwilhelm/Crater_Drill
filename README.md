# CRATER Drill Subsystem

Software for the drilling subsystem of Project CRATER's autonomous Mars rover.

## Repository Structure

### ESP32 Embedded Software

#### `drill_main`

Main firmware for autonomous drilling operations running on the drill controller ESP32.

#### `drill_testing`

Firmware for component-level testing and debugging of the drill system via CAN communication.

#### `sample_container_main`

Main firmware for autonomous sample container operation.

#### `sample_container_testing`

Firmware for testing and debugging of the sample container subsystem via CAN communication.

---

### ROS2 Integration

#### `drill_package`

ROS2 package responsible for integration of the drilling subsystem into the rover software framework.

Components:

* `drill_node` – ROS2 lifecycle node interfacing with the drilling hardware.
* `drill_logic` – State machine implementing drilling and sample collection logic.

#### `drill_interfaces`

Custom ROS2 message and service definitions used for drill commands, status reporting, and subsystem communication.

---

### Testing Tools

#### `drill_can_testing`

Python-based tools for CAN bus testing, debugging, and troubleshooting of the drilling subsystem.
