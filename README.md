# CoreXY Pen Plotter 

[![Build Status](https://img.shields.io/badge/Status-Completed-success.svg)](#)
[![Hardware](https://img.shields.io/badge/Platform-ESP32-blue.svg)](#)
[![Methodology](https://img.shields.io/badge/Engineering-V--Model_VDI_2206-orange.svg)](#)

> A low-cost, 2-axis CoreXY pen plotter engineered using V-Model (VDI) principles, built around the ESP32-NodeMCU.

![Hero Image or Demo GIF]([docs/images/demo.gif]) *<!-- Replace with a GIF or photo of your plotter in action -->*

## Overview

Developed as part of a 2-semester engineering elective, this project strictly adheres to **V-Model (VDI 2206)** development principles for mechatronic systems. The objective was to design, simulate, build, and test a functional pen plotter from scratch using a highly constrained Bill of Materials (BOM). 

By utilizing CoreXY kinematics, the design keeps the heavy DC motors stationary, reducing moving mass and allowing for faster, more precise plotting.

## Key Features
* **CoreXY Kinematics:** Coordinated motion control utilizing stationary motors.
* **Closed-Loop Feedback:** Real-time position tracking via IR slotted disc sensors.
* **Precise Pen Control:** Micro-servo actuated pen-lifting mechanism (Z-axis).
* **Automated Homing:** 4-point end-stop calibration for repeatable coordinate zeroing.
* **Wireless Potential:** Built on the ESP32 platform, leaving headroom for future WiFi/Bluetooth plotting interfaces.

---

## Hardware Bill of Materials (BOM)

| Component | Quantity | Purpose |
| :--- | :---: | :--- |
| **ESP32-NodeMCU** | 1 | Main microcontroller handling kinematics and IO |
| **Geared DC Motor** | 2 | Drives the CoreXY X/Y gantry belts |
| **DRV8833 Motor Driver** | 1 | Dual H-Bridge for controlling the DC motors |
| **9G Micro Servo Motor** | 1 | Actuates the pen (Z-axis up/down) |
| **IR Slotted Disc Sensor** | 2 | Odometry and position feedback for the DC motors |
| **XSS-5GL13 End-Stop Switch** | 4 | Physical limit detection and system homing |
| **MB102 Power Supply Module** | 1 | Breadboard power distribution for motors and logic |

---

## Engineering Methodology: V-Model (VDI 2206)

This project was built following the classic V-Model approach for mechatronics:
1. **Requirements & Specifications:** Defining plotting area, resolution, and component limits.
2. **System Design:** Mapping CoreXY equations to standard DC motor outputs.
3. **Domain-Specific Design:** Hardware circuit layout and ESP32 firmware architecture.
4. **Implementation:** Assembling the chassis, wiring the DRV8833, and writing the C++ control logic.
5. **Integration & Testing:** Calibrating IR sensor interrupts and tuning motor PWM signals.
6. **System Verification:** Homing sequence validation and final plot testing.

---

## Getting Started

### Prerequisites
* [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
* ESP32 Board Support Package installed

### Wiring Map
*Detailed schematic available in `[docs/schematic.pdf]`*

| ESP32 Pin | Component |
| :--- | :--- |
| `[Pin X]` | DRV8833 IN1 (Motor A) |
| `[Pin X]` | DRV8833 IN2 (Motor A) |
| `[Pin X]` | DRV8833 IN3 (Motor B) |
| `[Pin X]` | DRV8833 IN4 (Motor B) |
| `[Pin X]` | IR Sensor A (Interrupt) |
| `[Pin X]` | IR Sensor B (Interrupt) |
| `[Pin X]` | Servo PWM |
| `[Pins...]` | End-Stops |

### Installation
1. Clone the repository:
   ```bash
   git clone [https://github.com/Shivansh-Mehta/corexy_pen_plotter.git](https://github.com/Shivansh-Mehta/corexy_pen_plotter.git)
