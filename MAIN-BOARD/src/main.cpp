#include <Arduino.h>
// #include "../.pio/libdeps/uno/LiquidCrystal/src/LiquidCrystal.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define TEMP_START_CHAR 'T'
#define Humidity_START_CHAR 'H'
#define MAX_PWM 255
#define MOTOR_PIN1 9
#define MOTOR_PIN2 10

void readSerialBluetooth();
void process();
void updatePWM();
void printInLCD();

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float currentTemperature, currentHumidity;
bool sensorsValuesHaveChanged = false;
int wateringAmountCCPM = 0;
int wateringAmountPWM = 0;


void setup() {
	lcd.begin(20, 4);

	Serial.begin(9600);
}


void loop() {
	readSerialBluetooth();

	if(sensorsValuesHaveChanged) {
		process();
		updatePWM();
		printInLCD();
		sensorsValuesHaveChanged = false;
	}
}


void readSerialBluetooth(){
	if (Serial.available() < 5)  // 1 Byte First Char + 4 Bytes Float
		return;
  
	char firstChar = Serial.read();

	if(firstChar == TEMP_START_CHAR) {
		float oldTemperature = currentTemperature;
		currentTemperature = Serial.parseFloat();
		if(currentTemperature != oldTemperature)
			sensorsValuesHaveChanged = true;
	}
	else if(firstChar == Humidity_START_CHAR) {
		float oldHumidity = currentHumidity;
		currentHumidity = Serial.parseFloat();
		if(currentHumidity != oldHumidity)
			sensorsValuesHaveChanged = true;
	}
	else {  // Data is not right
		Serial.read();
	}
}


void process() {
	if(currentHumidity > 50) {
		wateringAmountCCPM = 0;
		wateringAmountPWM = 0;
	}
	else if(currentHumidity < 20) {
		wateringAmountCCPM = 20;
		wateringAmountPWM = MAX_PWM * 25 / 100;
	}
	else {
		if(currentTemperature < 25) {
			wateringAmountCCPM = 0;
			wateringAmountPWM = 0;
		}
		else{
			wateringAmountCCPM = 10;
			wateringAmountPWM = MAX_PWM * 10 / 100;
		}
	}
}


void updatePWM() {
	analogWrite(MOTOR_PIN1, wateringAmountPWM);
	analogWrite(MOTOR_PIN2, 0);
}


void printInLCD() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.println(String(currentTemperature).c_str());
	lcd.println(String(currentHumidity).c_str());
	lcd.setCursor(0, 1);
	if(wateringAmountCCPM == 0) {
		lcd.println("No Watering!");
	}
	else {
		lcd.println(String(wateringAmountCCPM));
		lcd.println("CC");
	}
}