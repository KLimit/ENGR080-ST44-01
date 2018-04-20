#ifndef __SPEAKER_CONTROL_H__
#define __SPEAKER_CONTROL_H__


#include <Arduino.h>
#include "DataSource.h"
#include "Pinouts.h"


class SpeakerControl : public DataSource{
    public:
        SpeakerControl(void);

        void init();
        void sendPulse();
        void writeDataBytes(unsigned char* buffer, size_t idx);

        
        const int delayTime = 20;
        int loopCounter = 0;
        unsigned long speakerTime;


};
#endif