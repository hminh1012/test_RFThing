#include <Wire.h>
#include "Adafruit_SHTC3.h"

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("SHTC3 test - RF210");

  // Khởi tạo I2C trên các chân của RF210
  Wire.begin(5, 6);  // SDA = GPIO5, SCL = GPIO6

  if (!shtc3.begin()) {
    Serial.println("Couldn't find SHTC3");
    Serial.println(" Kiểm tra lại dây SDA/SCL, nguồn 3.3V và GND.");
    while (1);
  }
  Serial.println("SHTC3 found!");
}

void loop() {
  sensors_event_t humidity, temp;
  shtc3.getEvent(&humidity, &temp);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.print(" °C  |  Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  delay(2000);
}