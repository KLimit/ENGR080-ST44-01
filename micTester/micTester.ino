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
MicrophoneADC mic;
Printer printer;

bool onetime = true;

void setup() {
    // prepare stuff
    pinMode(ENV_PIN, INPUT_PULLDOWN)
    attachInterrupt(ENV_PIN, isrEnvelope, RISING ); //This code is kind of weird. Depends on reaching 60% of input voltage.
    miclog.init();
    analogReadRes(8); // set to 8 bit resolution
    delay(1000);
<<<<<<< HEAD

=======
    // take one recording
    // miclog.log();
>>>>>>> 4d943de5d04032026a45b95e1731b16c8cf9adfb

}

void loop() {
    if (onetime) {
        // take one recording
        miclog.log();
        onetime = false;
    }
}

void isrEnvelope(){
    micLogger.envLog();

}
