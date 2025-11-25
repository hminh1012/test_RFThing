# RFThing Project

This project involves developing firmware for an ESP32-based IoT device (RFThing) that integrates LoRaWAN connectivity (via RAK3172), environmental sensing (SHTC3), and motion detection (KX023). It also features integration with the Blynk IoT platform for remote monitoring and control.
![c00561db-e13e-46f0-b7a2-3f3b5e9c03f3](https://github.com/user-attachments/assets/4806478e-26a7-4ef5-85d6-cd42747eee3c)

## Key Features

*   **LoRaWAN Connectivity**: Bridges ESP32 Serial to RAK3172 for AT command communication.
*   **Environmental Sensing**: Reads temperature and humidity from the SHTC3 sensor.
*   **Motion Sensing**: Reads acceleration data from the Kionix KX023 accelerometer.
*   **Fall Detection**: Implements a free-fall detection algorithm using the accelerometer.
*   **IoT Integration**: Connects to the Blynk platform to visualize sensor data and receive alerts (e.g., fall detection).
*   **Non-blocking Operation**: Uses timers for sensor reading and button debouncing to ensure smooth operation without `delay()`.

## File Descriptions

*   **`RF210_RAK3172_bridge.ino`**: A bridge sketch that allows direct communication with the RAK3172 module via the ESP32's USB serial port. Useful for configuring the LoRaWAN module using AT commands.
*   **`test_Blynk_complete.ino`**: The main application sketch that integrates:
    *   WiFi and Blynk connection.
    *   SHTC3 and KX023 sensor reading.
    *   Fall detection logic.
    *   Physical button handling with debouncing.
    *   Data transmission to the Blynk app.
*   **`test_Blynk.ino`**: A basic test sketch for Blynk connectivity.
*   **`test_Kionix_KX023.ino`**: A standalone test sketch for the KX023 accelerometer.
*   **`test_SHTC3.ino`**: A standalone test sketch for the SHTC3 temperature and humidity sensor.
*   **`Blynk_Email_Complete.ino`**: Likely a variation of the Blynk sketch with email notification features.

## Hardware Requirements

*   **MCU**: ESP32-C3 (or similar ESP32 variant).
*   **LoRa Module**: RAK3172.
*   **Sensors**:
    *   Adafruit SHTC3 (Temperature & Humidity).
    *   Kionix KX023 (Accelerometer).
*   **Other**: Push button, LED.

## Dependencies

The firmware requires the following Arduino libraries:
*   `WiFi`
*   `BlynkSimpleEsp32`
*   `Wire`
*   `Adafruit_SHTC3`
*   `Kionix_KX023`

## Setup & Usage

1.  **Install Libraries**: Install the required libraries via the Arduino Library Manager.
2.  **Configure Credentials**: Update `test_Blynk_complete.ino` with your WiFi credentials (`ssid`, `pass`) and Blynk Auth Token (`BLYNK_AUTH_TOKEN`).
3.  **Upload**: Select the correct board and port in the Arduino IDE and upload the desired sketch.
4.  **Monitor**: Use the Serial Monitor (115200 baud) to view debug output.

## Result

<img width="1290" height="732" alt="image" src="https://github.com/user-attachments/assets/25c07058-8822-4e95-ab1a-3609d4cf2499" />

<img width="1168" height="404" alt="image" src="https://github.com/user-attachments/assets/fe1e6374-f1fd-4aef-b08a-7c6546479684" />
