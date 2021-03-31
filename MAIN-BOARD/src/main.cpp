#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define TEMPERATURE_SIGNAL 'T'
#define HUMIDITY_SIGNAL 'H'
#define MAX_PWM 255
#define MOTOR_IN1 13
#define MOTOR_IN2 12
#define MOTOR_EN 11

void show_new_values_on_lcd();

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float current_temperature, current_humidity;
bool different_value_detected = false;
int watering_amount_in_cc_for_lcd = 0;
int watering_amount_for_pwm = 0;


void setup() {
	pinMode(MOTOR_IN1, OUTPUT);
	pinMode(MOTOR_IN2, OUTPUT);
	pinMode(MOTOR_EN, OUTPUT);

	digitalWrite(MOTOR_IN1, LOW);
	digitalWrite(MOTOR_IN2, HIGH);

	lcd.begin(20, 4);

	Serial.begin(9600);
}


void loop() 
{
	// Reading data written by bluetooth from Serial port 
	if (Serial.available() >= 7)
	{  
		char signal_character = Serial.read();

		if(signal_character == TEMPERATURE_SIGNAL) 
		{
			float previous_temperature = current_temperature;
			current_temperature = Serial.parseFloat();
			if(current_temperature != previous_temperature)
				different_value_detected = true;
		}
		else if (signal_character == HUMIDITY_SIGNAL) 
		{
			float previous_humidity = current_humidity;
			current_humidity = Serial.parseFloat();
			if(current_humidity != previous_humidity)
				different_value_detected = true;
		}

		Serial.readStringUntil('\n');
	}	

	if(different_value_detected) 
	{
		if(current_humidity > 50 || current_temperature < 25) 
		{
			watering_amount_in_cc_for_lcd = 0;
			watering_amount_for_pwm = 0;
		}
		else if(current_humidity < 20) {
			watering_amount_in_cc_for_lcd = 20;
			watering_amount_for_pwm = MAX_PWM * 25 / 100;
		}
		else if (current_temperature >= 25)
		{
			watering_amount_in_cc_for_lcd = 10;
			watering_amount_for_pwm = MAX_PWM * 10 / 100;
		}

		analogWrite(MOTOR_EN, watering_amount_for_pwm);

		show_new_values_on_lcd();
		different_value_detected = false;
	}
}

void show_new_values_on_lcd() 
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.println(String(current_temperature));
	lcd.println(String(current_humidity));
	lcd.setCursor(0, 1);
	if(watering_amount_in_cc_for_lcd == 0) 
	{
		lcd.println("no need for Watering.");
	}
	else 
	{
		lcd.println(String(watering_amount_in_cc_for_lcd));
		lcd.println("CC");
	}
}