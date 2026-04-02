# Arduino I2C Master-Slave Ultrasonic Monitor

This project demonstrates a dual-Arduino setup communicating via the I2C protocol. The Master Arduino reads distance data from an I2C ultrasonic sensor upon a button press, transmits this data to a Slave Arduino, and the Slave determines if an obstacle is within a critical threshold (< 200mm).

## Hardware Requirements
* **2x Arduino Boards** (e.g., Uno, Nano, or Mega)
* **1x I2C Ultrasonic Sensor** (Fixed address: `0x57`)
* **1x Push Button**
* **Jumper Wires** (Including I2C connections: SDA, SCL, and shared GND)

## Wiring Guide

### Master Arduino Connections:
* **Button:** Pin 2 to GND (uses internal pull-up resistor).
* **Ultrasonic Sensor:** SDA to Master SDA (A4 on Uno), SCL to Master SCL (A5 on Uno).
* **LED:** Uses built-in LED on Pin 13.

### Slave Arduino Connections:
* **LED:** Uses built-in LED on Pin 13.

### I2C Bus Connections (Crucial):
* Connect **Master SDA** to **Slave SDA**.
* Connect **Master SCL** to **Slave SCL**.
* Connect **Master GND** to **Slave GND** (The system will fail without a shared ground!).

## How It Works (System Logic)

1. **Trigger:** The system idles until the button on the Master is pressed.
2. **Sensor Read:** The Master requests 3 bytes of data from the I2C ultrasonic sensor and reconstructs the 24-bit distance value in millimeters.
3. **Transmission:** The Master sends this calculated distance (split into 2 bytes) over the I2C bus to the Slave (Address `0x08`).
4. **Logic Processing:** The Slave receives the distance. If the distance is less than 200mm, it flags `obstacleDetected = true` and turns on its LED.
5. **Feedback Loop:** The Master immediately requests 1 byte of data back from the Slave. The Slave replies with its `obstacleDetected` status (1 or 0). The Master then turns its own LED on or off to match the Slave.

## Code Structure
* `mastertest.ino`: Flashed to the primary Arduino. Handles the button interrupt, queries the ultrasonic sensor, and manages the I2C bus timing.
* `slavetest.ino`: Flashed to the secondary Arduino. Runs in the background waiting for `receiveEvent` or `requestEvent` from the Master to process thresholds and control the alert LED.

## Usage Instructions
1. Flash `mastertest.ino` to your first Arduino.
2. Flash `slavetest.ino` to your second Arduino.
3. Connect the I2C lines and ensure grounds are tied together.
4. Open the Serial Monitor for the Master board at `9600` baud.
5. Press the button to trigger a reading and watch the LEDs react to obstacles!
