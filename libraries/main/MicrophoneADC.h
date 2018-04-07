#ifndef __MICROPHONE_ADC_H__
#define __MICROPHONE_ADC_H__

#include <Arduino.h>
#include "MotorDriver.h"
#include "MicrophoneDataSource.h"
#include "Pinouts.h"

/*
 * MicrophoneADC implements SD logging for the ADC channels
 */

#define NUM_SAMPLES 8192
#define ENV_THRESHOLD 125  // threshold analog value for the envelope

class MicrophoneADC : public MicrophoneDataSource {
public:

  MicrophoneADC(void);

	// Check pboard for both of these
	int micPin = 17;
	int envPin = 14;


  // data to be stored
	unsigned long sampTime [NUM_SAMPLES];  // range: 0-(2^32-1), 4 bytes per item
  unsigned char mic [NUM_SAMPLES];  // range: 0-255, 1 byte per item
	unsigned long envThreshTime = 0;  // recorded when it fulfills a threshhold value

	// Managing state
  void updateSample(void);
  String printSample(void);

  // Write out
  void writeDataBytes(unsigned long * timeBuffer, unsigned char * micBuffer, unsigned long envBuffer);

  int lastExecutionTime = -1;
};
#endif