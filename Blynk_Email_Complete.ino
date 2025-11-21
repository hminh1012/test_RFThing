/*
 * FIXED VERSION: Correct order of Includes vs Defines
 * * 1. BLYNK DEFINES (MUST BE FIRST)
 * 2. LIBRARIES
 * 3. SENSOR & EMAIL LOGIC
 */

// -------------------------------------------------------------------------
// 1. BLYNK CONFIGURATION (MUST BE THE VERY FIRST LINES)
// -------------------------------------------------------------------------
#define BLYNK_TEMPLATE_ID "TMPL6lpLMLUzT"
#define BLYNK_TEMPLATE_NAME "RFthing"
#define BLYNK_AUTH_TOKEN "P7rRuFHui5Dt0rTZZCeit-tfOhU152h3"
#define BLYNK_PRINT Serial

// -------------------------------------------------------------------------
// 2. INCLUDE LIBRARIES
// -------------------------------------------------------------------------
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // Now safe to include because ID is defined above
#include <Wire.h>
#include "Adafruit_SHTC3.h"
#include <Kionix_KX023.h>
#include <ESP_Mail_Client.h> 

// -------------------------------------------------------------------------
// 3. EMAIL CONFIGURATION
// -------------------------------------------------------------------------
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "pidrowsiness@gmail.com"
#define AUTHOR_PASSWORD "fjxl mvzd kzmu piig" 
#define RECIPIENT_EMAIL "tranhoangminh675@gmail.com"

// --- WiFi Credentials ---
char ssid[] = "Bubuchacha";
char pass[] = "umbalaxibua";

// --- Pin Definitions ---
#define LED 4       
#define BUTTON 3    
#define VIRTUAL_PIN_FALL_DETECT V11

// --- Global Objects ---
BlynkTimer timer;
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
KX023 myIMU;
SMTPSession smtp; 

// --- Global Variables for Data ---
float kx_x, kx_y, kx_z; 
float currentTemp = 0.0; 
float currentHum = 0.0; 

// --- Logic Constants ---
const float FREE_FALL_THRESHOLD = 0.3; 
bool isFalling = false; 

// --- Button Debounce ---
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

// -------------------------------------------------------------------------
// EMAIL FUNCTIONS
// -------------------------------------------------------------------------

// Callback to print email status
void smtpCallback(SMTP_Status status){
  Serial.println(status.info());
  if (status.success()){
    Serial.println("----------------");
    Serial.printf("Email sent successfully!\n");
    Serial.println("----------------");
  }
  smtp.sendingResult.clear();
}

void sendFallAlert() {
  Serial.println("Preparing to send Emergency Email...");

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";
  
  // Set Time for Vietnam (GMT+7)
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 7; 
  config.time.day_light_offset = 0;

  SMTP_Message message;
  message.sender.name = F("Smart Monitor");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("URGENT: Fall Detected!");
  message.addRecipient(F("Admin"), RECIPIENT_EMAIL);

  // HTML Body
  String htmlMsg = "<div style=\"color:#D8000C; background-color: #FFBABA; padding: 20px;\">";
  htmlMsg += "<h1>⚠️ FALL DETECTED ⚠️</h1>";
  htmlMsg += "<p>The device has detected a free-fall event.</p>";
  htmlMsg += "<h3>Current Status:</h3>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><b>Temperature:</b> " + String(currentTemp) + " °C</li>";
  htmlMsg += "<li><b>Humidity:</b> " + String(currentHum) + " %</li>";
  htmlMsg += "<li><b>Impact Force (Vector):</b> " + String(sqrt(pow(kx_x, 2) + pow(kx_y, 2) + pow(kx_z, 2))) + " g</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<p>Please check the device or the user immediately.</p>";
  htmlMsg += "</div>";

  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  if (!smtp.connect(&config)){
    Serial.printf("Connection error: %s\n", smtp.errorReason().c_str());
    return;
  }
  if (!MailClient.sendMail(&smtp, &message))
    Serial.printf("Error sending Email: %s\n", smtp.errorReason().c_str());
}

// -------------------------------------------------------------------------
// BLYNK & SENSOR FUNCTIONS
// -------------------------------------------------------------------------

BLYNK_WRITE(V0) {
  int p = param.asInt();
  digitalWrite(LED, p);
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
}

void sendAllSensorData() {
  sensors_event_t humidity, temp;
  shtc3.getEvent(&humidity, &temp);

  currentTemp = temp.temperature;
  currentHum = humidity.relative_humidity;

  Serial.print("Temp: "); Serial.print(currentTemp);
  Serial.print(" °C | Hum: "); Serial.print(currentHum);
  Serial.print(" % | Accel X: "); Serial.println(kx_x); 

  Blynk.virtualWrite(V5, currentTemp);        
  Blynk.virtualWrite(V6, currentHum); 
  Blynk.virtualWrite(V7, kx_x); 
  Blynk.virtualWrite(V8, kx_y);                        
  Blynk.virtualWrite(V9, kx_z);                        
}

void checkPhysicalButton() {
  int currentButtonState = digitalRead(BUTTON);
  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState != lastButtonState) {
      if (currentButtonState == LOW) {
        Serial.println("Physical button pressed!");
        int currentLedState = digitalRead(LED);
        digitalWrite(LED, !currentLedState);
        Blynk.virtualWrite(V0, digitalRead(LED));
      }
      lastButtonState = currentButtonState;
    }
  }
}

void detectFreeFall() {
  myIMU.readAsynchronousReadBackAccelerationData(&kx_x, &kx_y, &kx_z);
  float totalAcceleration = sqrt(pow(kx_x, 2) + pow(kx_y, 2) + pow(kx_z, 2));

  if (totalAcceleration < FREE_FALL_THRESHOLD) {
    if (!isFalling) {
      isFalling = true; 
      Serial.println("!!! FREE FALL DETECTED !!!");
      
      Blynk.virtualWrite(VIRTUAL_PIN_FALL_DETECT, 1);
      Blynk.virtualWrite(V12, totalAcceleration); 
      
      sendFallAlert(); // Send email
    }
  } else {
    if (isFalling) {
      isFalling = false; 
      Serial.println("Fall ended.");
      Blynk.virtualWrite(VIRTUAL_PIN_FALL_DETECT, 0);
    }
  }
}

// -------------------------------------------------------------------------
// SETUP & LOOP
// -------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  delay(100); 
  
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Wire.begin(5, 6); 

  if (!shtc3.begin()) { 
    Serial.println("Couldn't find SHTC3");
    while (1);
  }

  KX023_Status_t status = myIMU.begin();
  if (status != KX023_STATUS_OK) {
    Serial.println("KX023 Failed");
    while(1) delay(10);
  }
  myIMU.configAsynchronousReadBackAccelerationData(KX023_ACCLERATION_RANGE_2G, KX023_ODR_25HZ);
  myIMU.setOperatingMode();
  
  smtp.debug(1); 
  smtp.callback(smtpCallback);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendAllSensorData);
  timer.setInterval(20L, checkPhysicalButton);
  timer.setInterval(50L, detectFreeFall);
}

void loop() {
  Blynk.run();
  timer.run();
}
