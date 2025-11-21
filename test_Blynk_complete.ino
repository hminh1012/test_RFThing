/*
 * This is the fixed, combined sketch.
 * It reads both sensors AND checks the physical button using non-blocking timers.
 * This removes all delays from the loop, fixing the sensor "glitch".
 */

// --- Blynk Defines ---
#define BLYNK_TEMPLATE_ID "TMPL6lpLMLUzT"
#define BLYNK_TEMPLATE_NAME "RFthing"
#define BLYNK_AUTH_TOKEN "P7rRuFHui5Dt0rTZZCeit-tfOhU152h3"
#define BLYNK_PRINT Serial
#define APP_DEBUG




// --- Required Libraries ---
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>               // For I2C communication
#include "Adafruit_SHTC3.h"     // For SHTC3 sensor
#include <Kionix_KX023.h>       // For KX023 sensor 

// --- Pin Definitions ---
#define LED 4       // GPIO4 - LED
#define BUTTON 3    // GPIO3 - Nút nhấn
// --- Blynk Virtual Pins ---
#define VIRTUAL_PIN_FALL_DETECT V11

// --- WiFi Credentials ---
char ssid[] = "Bubuchacha";
char pass[] = "umbalaxibua";

// --- Global Objects & Variables ---
BlynkTimer timer;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
KX023 myIMU;
float kx_x, kx_y, kx_z; // Global variables for KX023 data


// The total acceleration vector magnitude must be below this value to be considered a fall.
const float FREE_FALL_THRESHOLD = 0.3; // Value in 'g'
bool isFalling = false; // **NEW** State variable for fall detection


// --- State variables for non-blocking button check ---
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50ms debounce time

// 📲 Nhận dữ liệu từ App Blynk (Virtual Pin V0)
BLYNK_WRITE(V0)
{
  int p = param.asInt();
  Serial.print("Blynk received value on V0: ");
  Serial.println(p);
  digitalWrite(LED, p);
}

/**
 * Runs when ESP32 first connects to Blynk.
 * Syncs the LED state with the button in the app.
 */
BLYNK_CONNECTED()
{
  Serial.println("Blynk Connected. Syncing V0...");
  Blynk.syncVirtual(V0);
}


/**
 * Timer Function 1 (runs every 1000ms)
 * Reads data from BOTH sensors and sends to Blynk.
 */
void sendAllSensorData()
{
  // 1. Read SHTC3
  sensors_event_t humidity, temp;
  shtc3.getEvent(&humidity, &temp);

  // 2. Read KX023
  myIMU.readAsynchronousReadBackAccelerationData(&kx_x, &kx_y, &kx_z);

  // 3. Print values to Serial Monitor for debugging
  Serial.print("Temp: ");
  Serial.print(temp.temperature);
  Serial.print(" °C | Hum: ");
  Serial.print(humidity.relative_humidity);
  Serial.print(" % | X: ");
  Serial.print(kx_x);
  Serial.print(" | Y: ");
  Serial.print(kx_y);
  Serial.print(" | Z: ");
  Serial.println(kx_z);

  // 4. Send all data to Blynk
  Blynk.virtualWrite(V4, 0);  
  Blynk.virtualWrite(V5, temp.temperature);        // SHTC3 Temp
  Blynk.virtualWrite(V6, humidity.relative_humidity);  // SHTC3 Humidity
  Blynk.virtualWrite(V7, kx_x);                        // KX023 X-Axis
  Blynk.virtualWrite(V8, kx_y);                        // KX023 Y-Axis
  Blynk.virtualWrite(V9, kx_z);                        // KX023 Z-Axis
}

/**
 * Timer Function 2 (runs every 20ms)
 * Checks the physical button without blocking the code.
 */
void checkPhysicalButton()
{
  int currentButtonState = digitalRead(BUTTON);

  // Check if the button state has changed (pressed or released)
  if (currentButtonState != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the button has been in the new state for long enough (50ms)
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the state has *stably* changed
    if (currentButtonState != lastButtonState) {
      
      // Check if the button was just pressed (went from HIGH to LOW)
      if (currentButtonState == LOW) {
        Serial.println("Physical button pressed!");
        
        // Toggle the LED
        int currentLedState = digitalRead(LED);
        digitalWrite(LED, !currentLedState);
        
        // Send the NEW state back to the Blynk app
        Blynk.virtualWrite(V0, digitalRead(LED));
      }
      
      // Save the new state
      lastButtonState = currentButtonState;
    }
  }
}


void detectFreeFall()
{
  // 1. Read KX023 acceleration data (it's already read asynchronously, so this is fast)
  // The global variables kx_x, kx_y, kx_z are updated by sendAllSensorData, 
  // but to ensure the most frequent check, we should re-read them here
  // or use a faster timer for sendAllSensorData if needed.
  // For simplicity and quick implementation, let's re-read the data:
  myIMU.readAsynchronousReadBackAccelerationData(&kx_x, &kx_y, &kx_z);

  // 2. Calculate the total vector magnitude of the acceleration (Magnitude = sqrt(x^2 + y^2 + z^2))
  float totalAcceleration = sqrt(pow(kx_x, 2) + pow(kx_y, 2) + pow(kx_z, 2));

  // 3. Check for Free-Fall condition
  if (totalAcceleration < FREE_FALL_THRESHOLD) {
    // Condition met: Acceleration is near 0g (free fall)
    if (!isFalling) {
      // **NEW FALL EVENT**
      isFalling = true; // Set state
      Serial.println("!!! FREE FALL DETECTED !!!");
      
      // Trigger V11 with a '1' (or a message, timestamp, etc.)
      Blynk.virtualWrite(VIRTUAL_PIN_FALL_DETECT, 1);
      
      // Optional: Send current acceleration values for context
      Blynk.virtualWrite(V12, totalAcceleration); 
    }
  } else {
    // Not in free-fall
    if (isFalling) {
      // **FALL ENDED**
      isFalling = false; // Reset state
      Serial.println("Fall ended.");
      
      // Reset V11 with a '0' (or use an event-based system in Blynk)
      Blynk.virtualWrite(VIRTUAL_PIN_FALL_DETECT, 0);
    }
  }
}



void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(100); 
  Serial.println("SHTC3 + KX023 + Blynk Test (No-Delay Version)");
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // --- Initialize I2C Bus on custom pins ---
  Wire.begin(5, 6); // SDA = GPIO5, SCL = GPIO6 

  // --- Initialize SHTC3 Sensor ---
  if (!shtc3.begin()) { 
    Serial.println("Couldn't find SHTC3 🙁");
    while (1);
  }
  Serial.println("SHTC3 found!");

  // --- Initialize KX023 Sensor ---
  KX023_Status_t status = myIMU.begin();
  if (status == KX023_STATUS_OK)
  {
    Serial.println("KX023-1025: OK");
  }
  else
  {
    Serial.print("KX023-1025: FAILED. Status code: ");
    Serial.println(status);
    while(1) delay(10); // Halt on failure
  }

  // Configure KX023 sensor
  myIMU.configAsynchronousReadBackAccelerationData(KX023_ACCLERATION_RANGE_2G, KX023_ODR_25HZ);
  myIMU.setOperatingMode();
  delay(50); // This delay in setup() is OK

  // --- Connect to Blynk ---
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // --- Setup Timers ---
  // Timer 1: Send sensor data every 1 second (1000ms)
  timer.setInterval(1000L, sendAllSensorData);
  
  // Timer 2: Check the physical button every 20ms
  timer.setInterval(20L, checkPhysicalButton);
  //Timer 3: Check for Free Fall frequently (50ms)
  timer.setInterval(50L, detectFreeFall);
}

void loop()
{
  // This is ALL that should be in your loop.
  // These functions process all timers and server communication.
  Blynk.run();
  timer.run();
}
