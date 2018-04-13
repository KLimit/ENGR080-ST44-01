#ifndef __ENV_ADC_H__
#define __ENV_ADC_H__

#include <Arduino.h>
// #include "MotorDriver.h"
// #include "MicrophoneDataSource.h"
#include "Pinouts.h"
#include "DataSource.h"

/*
 * EnvADC implements SD logging for the ADC channels
 */

#define NUM_SAMPLES 512
#define ENV_THRESHOLD 125  // threshold analog value for the envelope



class EnvADC :public DataSource {
public:

  EnvADC(void);

	// Check pboard for both of these
	// int micPin = 17;
	// int envPin = 14;


  // data to be stored
  volatile int sample = 0;

	// unsigned long sampTime [NUM_SAMPLES];  // range: 0-(2^32-1), 4 bytes per item
  // unsigned char mic [NUM_SAMPLES];  // range: 0-255, 1 byte per item
	// unsigned long envThreshTime = 0;  // recorded when it fulfills a threshhold value

	// Managing state
  void updateSample(void);
  String printSample(void);

  // Write out
  size_t writeDataBytes(unsigned char * buffer, size_t idx);
  // void writeDataBytes(unsigned char * timeBuffer, unsigned char * micBuffer, unsigned char * envBuffer);

};
#endif
