/******************************************************************************
 * Combined SHTC3 + Kionix KX023 Sensor Uplink to Blynk
 * * This code is for an ESP32-C3 microcontroller.
 * * It reads:
 * 1. Temperature & Humidity from an Adafruit SHTC3 sensor
 * 2. X, Y, Z acceleration from a Kionix KX023 sensor
 * * It connects to WiFi and uploads all 5 data points to the Blynk cloud.
 * * I2C WIRING (for both sensors on RF210):
 * - SDA: GPIO 5
 * - SCL: GPIO 6
 * * BLYNK VIRTUAL PINS:
 * - V0: Temperature (°C)
 * - V1: Humidity (%)
 * - V2: Acceleration X (g)
 * - V3: Acceleration Y (g)
 * - V4: Acceleration Z (g)
 * * REQUIRED LIBRARIES (Install via Arduino Library Manager):
 * 1. Blynk by Volodymyr Shymanskyy
 * 2. Adafruit SHTC3 Library by Adafruit
 * 3. Kionix_KX023 (You should already have this from your example)
 ******************************************************************************/

// --- 1. BLYNK & WIFI DEFINITIONS ---
// Define BLYNK_PRINT to redirect Blynk logs to Serial
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include "Adafruit_SHTC3.h"
#include <Kionix_KX023.h>

// --- !! IMPORTANT !! ---
// FILL IN YOUR BLYNK & WIFI CREDENTIALS HERE
// You get these from your Blynk project
char auth[] = "YOUR_BLYNK_AUTH_TOKEN"; 
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// --- 2. SENSOR OBJECTS & GLOBAL VARIABLES ---

// SHTC3 Sensor Object
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

// Kionix Sensor Object
KX023 myIMU;
float kx_x, kx_y, kx_z; // Global variables for accelerometer data

// Blynk Timer object
BlynkTimer timer;

// --- 3. SETUP FUNCTION ---
// This runs once when the board boots up
void setup() {
  Serial.begin(115200);
  delay(100); // Wait for serial to initialize
  Serial.println("Starting SHTC3 + Kionix + Blynk Uploader...");

  // Start Blynk connection
  Blynk.begin(auth, ssid, pass);

  // Initialize I2C bus ONCE for both sensors
  // Using GPIO 5 (SDA) and GPIO 6 (SCL) as per your files
  Wire.begin(5, 6); 

  // --- Initialize SHTC3 ---
  Serial.println("Initializing SHTC3 (Temp/Humidity)...");
  if (!shtc3.begin()) {
    Serial.println("Couldn't find SHTC3 sensor!");
    Serial.println("Check wiring (SDA/SCL, 3.3V, GND). Halting.");
    while (1) delay(10); // Stop forever
  }
  Serial.println("SHTC3 Found!");

  // --- Initialize Kionix KX023 ---
  Serial.println("Initializing Kionix KX023 (Accelerometer)...");
  KX023_Status_t status = myIMU.begin();
  if (status == KX023_STATUS_OK) {
    Serial.println("KX023-1025: OK");
  } else {
    Serial.print("KX023-1025: FAILED. Status code: ");
    Serial.println(status);
    Serial.println("Check wiring (SDA/SCL, 3.3V, GND). Halting.");
    while (1) delay(10); // Stop forever
  }

  // Configure Kionix sensor as per your example
  myIMU.configAsynchronousReadBackAccelerationData(KX023_ACCLERATION_RANGE_2G, KX023_ODR_25HZ);
  myIMU.setOperatingMode();
  delay(50);
  Serial.println("All sensors initialized.");

  // --- 4. BLYNK TIMER SETUP ---
  // Setup a function to be called every 2 seconds (2000 milliseconds)
  // This is better than using delay() in the loop
  timer.setInterval(2000L, sendSensorData);
}

// --- 5. DATA SENDING FUNCTION ---
// This function is called by the timer
void sendSensorData() {
  
  // --- Read SHTC3 ---
  sensors_event_t humidity_event, temp_event;
  shtc3.getEvent(&humidity_event, &temp_event);

  // --- Read Kionix KX023 ---
  myIMU.readAsynchronousReadBackAccelerationData(&kx_x, &kx_y, &kx_z);

  // --- Print to Serial Monitor (for debugging) ---
  Serial.print("Temperature: ");
  Serial.print(temp_event.temperature);
  Serial.print(" °C  |  Humidity: ");
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print("Accel X: ");
  Serial.print(kx_x);
  Serial.print(" g | Y: ");
  Serial.print(kx_y);
  Serial.print(" g | Z: ");
  Serial.print(kx_z);
  Serial.println(" g");

  // --- Send data to Blynk Cloud ---
  Blynk.virtualWrite(V0, temp_event.temperature);
  Blynk.virtualWrite(V1, humidity_event.relative_humidity);
  Blynk.virtualWrite(V2, kx_x);
  Blynk.virtualWrite(V3, kx_y);
  Blynk.virtualWrite(V4, kx_z);
}

// --- 6. MAIN LOOP ---
// This runs continuously
void loop() {
  Blynk.run(); // This keeps the board connected to Blynk
  timer.run(); // This checks the timer and runs sendSensorData() if it's time
}
