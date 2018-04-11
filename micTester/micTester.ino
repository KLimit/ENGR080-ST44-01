#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>
#include <MicLogger.h>
// #include <MicrophoneADC.h>
#include <EnvADC.h>
#include <Printer.h>
#include <Logger.h>
#include <ADCSampler.h>


MicLogger micLogger;
// MicrophoneADC mic;
Printer printer;
EnvADC env;

int counting = 0;
void setup() {
    // prepare stuff
    Serial.begin(9600);
    Serial.println("Serial Initialized");
    pinMode(ENV_PIN, INPUT_PULLDOWN); //Should I use pulldown? Probably
    Serial.println("pinMode set up");
    attachInterrupt(ENV_PIN, isrEnvelope, RISING ); //This code is kind of weird. Depends on reaching 60% of input voltage.
    Serial.println("Interrupt complete!");
    micLogger.init();
    Serial.println("MicLogger complete!");
    // analogReadRes(8); // set to 8 bit resolution--No need if using the envelope method? 
    delay(1000);

    // take one recording
    // mic.updateSample();
    // micLogger.log(); 

}

void loop() {
    delay(1000);
   Serial.println(counting);
   counting++; 
    
    
}

void isrEnvelope(){
   int startTime = micros();
    micLogger.envLog();
    Serial.println(micros() - startTime);

}
