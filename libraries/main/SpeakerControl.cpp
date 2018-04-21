#include "SpeakerControl.h"
#include "Printer.h"
extern Printer printer;
#include "Pinouts.h"

SpeakerControl::SpeakerControl(void)
:DataSource("SpeakerTime", "ulong"){
}

void SpeakerControl::init(void){
    pinMode(SPEAKER_PIN, OUTPUT);
    digitalWrite(SPEAKER_PIN, LOW);
}


void SpeakerControl::sendPulse(void){
    if(loopCounter%50 == 0 && playSwitch == false){
        speakerTime = micros();
        digitalWrite(SPEAKER_PIN, HIGH);
        playSwitch = true;
        
    }
    else{
        if(playTimeCounter>10){
            digitalWrite(SPEAKER_PIN, LOW);
            playTimeCounter = 0;
            playSwitch = false; 
        }
        delay(delayTime);
    }
    if(playSwitch == true){
        playTimeCounter++;  
    }
    loopCounter++;
    
}

size_t SpeakerControl::writeDataBytes(unsigned char* buffer, size_t idx){
    unsigned long * time_slot = (unsigned long *) (buffer+idx);
    time_slot[0] = speakerTime;
    return idx + sizeof(unsigned long);

}