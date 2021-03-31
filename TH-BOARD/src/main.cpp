#include <Arduino.h>
#include <Wire.h>

#define SHT25_ADDR 0x40

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  unsigned int data[2];

  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(0xF5);
  Wire.endTransmission();
  Wire.requestFrom(SHT25_ADDR, 2);
  // Read 2 bytes of data
  // humidity msb, humidity lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    // Convert the data
    float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
    // Output data to Serial Monitor
    Serial.print("Relative Humidity :");
    Serial.print(humidity);
    Serial.println(" %RH");
  }
  // put your main code here, to run repeatedly:

  Wire.beginTransmission(SHT25_ADDR);
  // Send temperature measurement command, NO HOLD master
  Wire.write(0xF3);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);
  // Request 2 bytes of data
  Wire.requestFrom(SHT25_ADDR, 2);
  // Read 2 bytes of data
  // temp msb, temp lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    // Convert the data
    float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
    float fTemp = (cTemp * 1.8) + 32;
    // Output data to Serial Monitor
    Serial.print("Temperature in Celsius :");
    Serial.print(cTemp);
    Serial.println(" C");
    Serial.print("Temperature in Fahrenheit :");
    Serial.print(fTemp);
    Serial.println(" F");

    // send every 5 second
    delay(5000);
  }
}