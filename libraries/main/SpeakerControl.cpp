#include "SpeakerControl.h"
#include "Printer.h"
extern Printer printer;
#include "Pinouts.h"

SpeakerControl::SpeakerControl(void)
:DataSource("SpeakerTime", "ulong"){
}

void SpeakerControl::init(void){
    pinMode(SPEAKER_PIN, OUTPUT);
}


void SpeakerControl::sendPulse(void){
    if(loopCounter%20 == 0){
        speakerTime = micros();
        digitalWrite(SPEAKER_PIN, HIGH);
        delay(delayTime);
        digitalWrite(SPEAKER_PIN, LOW);
    }
    else{
        delay(delayTime);
    }
    loopCounter++;
}

size_t SpeakerControl::writeDataBytes(unsigned char* buffer, size_t idx){
    unsigned long * time_slot = (unsigned long *) (buffer+idx);
    time_slot[0] = speakerTime;
    return idx + sizeof(unsigned long);

}