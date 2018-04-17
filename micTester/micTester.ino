#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>
#include <Logger.h>
#include <EnvADC.h>
#include <Printer.h>

Logger logger;
EnvADC env;
Printer printer;

void setup() {
    // prepare stuff
    Serial.begin(9600);
    Serial.println("Serial Initialized");

    logger.include(&env);

    pinMode(ENV_PIN, INPUT_PULLDOWN); //Should I use pulldown? Probably
    Serial.println("pinMode set up");

    attachInterrupt(ENV_PIN, isrEnvelope, RISING ); //This code is kind of weird. Depends on reaching 60% of input voltage.
    Serial.println("Interrupt complete!");

    logger.init();
    Serial.println("Logger Initialized!");
    delay(1000);
	}

void loop() {
  delay(1000);
  logger.log();
}

void isrEnvelope(){
    env.updateSample();
    Serial.println("INTERRUPT TRIGGERED");
}
