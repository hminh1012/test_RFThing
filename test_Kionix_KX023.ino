#include <Wire.h>
#include <Kionix_KX023.h>

KX023 myIMU;
float kx_x, kx_y, kx_z;

void setup(void)
{
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial
  }

  // * MODIFICATION 1: Use I2C pins 5 (SDA) and 6 (SCL) for your RF210 *
  Wire.begin(5, 6);

  Serial.println("KX023-1025 Asynchronous Read Back (for RF210)");

  KX023_Status_t status = myIMU.begin();
  if (status == KX023_STATUS_OK)
  {
    Serial.println("KX023-1025: OK");
  }
  else
  {
    // * MODIFICATION 2: Replaced kx023_get_status_string() as it's not defined in this snippet *
    Serial.print("KX023-1025: FAILED. Status code: ");
    Serial.println(status);
    Serial.println("→ Check wiring on GPIO5 (SDA) and GPIO6 (SCL).");
    while(1) delay(10); // Halt on failure
  }

  // Configure sensor as per your example
  myIMU.configAsynchronousReadBackAccelerationData(KX023_ACCLERATION_RANGE_2G, KX023_ODR_25HZ);

  myIMU.setOperatingMode();
  delay(50);
}

void loop(void)
{
  myIMU.readAsynchronousReadBackAccelerationData(&kx_x, &kx_y, &kx_z);
  Serial.print(kx_x);
  Serial.print(", ");
  Serial.print(kx_y);
  Serial.print(", ");
  Serial.print(kx_z);
  Serial.println("");

  // * MODIFICATION 3: Added a delay to prevent flooding the Serial Monitor *
  delay(200); // Wait 200ms between readings
}