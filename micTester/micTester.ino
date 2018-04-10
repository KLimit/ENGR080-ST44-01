#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>
#include <MicLogger.h>
#include <MicrophoneADC.h>
#include <Printer.h>


MicLogger miclog;
MicrophoneADC mic;
Printer printer;

bool onetime = true;

void setup() {
    // prepare stuff
    miclog.init();
    analogReadRes(8); // set to 8 bit resultion
    delay(1000);


}

void loop() {
    if (onetime) {
        // take one recording
        miclog.log();
        onetime = false;
    }
}

