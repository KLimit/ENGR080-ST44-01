#include "MicrophoneADC.h"
#include <math.h>
#include "Printer.h"
extern Printer printer;

MicrophoneADC::MicrophoneADC(void)
  : Microphone	DataSource("A00,A01,A02,A03,A10,A11,A12,A13,A14,A15,A16,A17,A18,A19",
               "int,int,int,int,int,int,int,int,int,int,int,int,int,int") // from DataSource
{}


void MicrophoneADC::updateSample(void)
{
	bool triggered = false;
	unsigned char env = 0;  // for checking the threshold envelope value
  for (int i=0; i<NUM_SAMPLES; i++){
     // Record time and analog values of micPin
		 sampTime[i] = (unsigned long) micros();  // casting to be safe
		 mic[i] = (unsigned char) analogRead(micPin);
		 env = (unsigned char) analogRead(envPin);
		 if (env > ENV_THRESHOLD && !triggered) {
			 envThreshTime = micros();
			 triggered = true;
		 }
  }
}

// String MicrophoneADC::printSample(void)
// {
//   String printString = "ADC:";
//   for (int i=0; i<NUM_PINS; i++) {
//     printString += " ";
//     printString += String(sample[i]);
//   }
//   return printString; //printer.printValue(0, printString);
// }

void MicrophoneADC::writeDataBytes(unsigned char * timeBuffer, unsigned char * micBuffer, unsigned char * envBuffer)
{
  unsigned long * data_time = (unsigned long *) &timeBuffer[0];
	unsigned char * data_mic = (unsigned char *) &micBuffer[0];
	unsigned char * data_env = (unsigned char *) &envBuffer;
  for (int i=0; i<NUM_SAMPLES; i++) {

		//Casting an unsigned long as an unsigned char takes the 8 least significant bits and truncates the rest.
			data_time[4i] = (unsigned char) sampTime[i]>>24; 
			data_time[4i+1] = (unsigned char) sampTime[i]>>16;
			data_time[4i+2] = (unsigned char) sampTime[i]>>8;
			data_time[4i+3] = (unsigned char) sampTime[i];
			data_mic[i] = mic[i];
	}
	
	for(int i = 4; i > 0; i--){
		data_env[i] = (unsigned char) envBuffer>>(i-1)*8;
	}


}
