#ifndef __MICROPHONE_ADC_H__
#define __MICROPHONE_ADC_H__

#include <Arduino.h>
#include "MotorDriver.h"
// #include "MicrophoneDataSource.h"
#include "Pinouts.h"

/*
 * MicrophoneADC implements SD logging for the ADC channels
 */

#define NUM_SAMPLES 512*2^2
#define ENV_THRESHOLD 125  // threshold analog value for the envelope
typedef struct {

  unsigned long time;
  unsigned char mic;
  unsigned char envtime;
}mic_sample_t;


class MicrophoneADC {
public:

  MicrophoneADC(void);

	// Check pboard for both of these
	// int micPin = 17;
	// int envPin = 14;


  // data to be stored
  mic_sample_t samples[NUM_SAMPLES];

	// unsigned long sampTime [NUM_SAMPLES];  // range: 0-(2^32-1), 4 bytes per item
  // unsigned char mic [NUM_SAMPLES];  // range: 0-255, 1 byte per item
	// unsigned long envThreshTime = 0;  // recorded when it fulfills a threshhold value

	// Managing state
  void updateSample(void);
  String printSample(void);

  // Write out
  void writeDataBytes(unsigned char * buffer);
  // void writeDataBytes(unsigned char * timeBuffer, unsigned char * micBuffer, unsigned char * envBuffer);

  int lastExecutionTime = -1;
};
#endif
