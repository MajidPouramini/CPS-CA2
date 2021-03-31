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
  unsigned int start = millis();
  Serial.println(start);
  
  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(0xF5);
  Wire.endTransmission();

  delay(500);

  Wire.requestFrom(SHT25_ADDR, 2);

  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
    Serial.print("H");
    Serial.println(humidity);
  }

  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(0xF3);
  Wire.endTransmission();

  delay(500);

  Wire.requestFrom(SHT25_ADDR, 2);

  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  
    float temperature = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
  
    Serial.print("T");
    Serial.println(temperature);
  }

  while(millis() - start < 5000);
}