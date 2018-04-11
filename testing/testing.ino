#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>


int timesTriggered = 0;

void setup() {
  // put your setup code here, to run once:
	pinMode(9, INPUT_PULLDOWN);  // makes sure 0V
	Serial.begin(9600);
	delay(1000);
	Serial.println("SERIAL INITIALIZED");

	attachInterrupt(9, isrTrigger, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
	Serial.println("not triggered yet");
	delay(5000);
}

void isrTrigger() {
	timesTriggered = timesTriggered + 1;
	Serial.println("PIN 9 TRIGGERED: " + timesTriggered);
	delay(1000);
}
