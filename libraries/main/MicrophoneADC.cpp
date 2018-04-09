#include "MicrophoneADC.h"
#include <math.h>
#include "Printer.h"
extern Printer printer;

MicrophoneADC::MicrophoneADC(void){}


void MicrophoneADC::updateSample(void)
{
	bool triggered = false;
   // for checking the threshold envelope value
  for (int i=0; i<NUM_SAMPLES; i++){
     // Record time and analog values of micPin
		 samples[i].time  =  micros();  // casting to be safe
		 samples[i].mic = (unsigned char) analogRead(micPin); //CHANGE THE ANALOG PIN RESOLUTION TO 8 BITS
		 if (samples[i].mic > ENV_THRESHOLD && !triggered) {
			 samples[i].envtime = 1;
			 triggered = true;
		 }
		 else{
			 samples[i].envtime = 0;
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

void MicrophoneADC::writeDataBytes(unsigned char * buffer)
{
  mic_sample_t* data_slot = (mic_sample_t*) &buffer[0];
  for (int i=0; i<NUM_SAMPLES; i++) {
		data_slot[i] = samples[i];
	}
}

// void MicrophoneADC::writeDataBytes(unsigned char * timeBuffer, unsigned char * micBuffer, unsigned char * envBuffer)
// {
//   unsigned char * data_time = (unsigned char *) &timeBuffer[0]; //Why do we take the address? I'm not sure. Saving memory?
// 	unsigned char * data_mic = (unsigned char *) &micBuffer[0];
// 	unsigned char * data_env = (unsigned char *) &envBuffer;
//   for (int i=0; i<NUM_SAMPLES; i++) {

// 		//Casting an unsigned long as an unsigned char takes the 8 least significant bits and truncates the rest.
// 			data_time[4i] = (unsigned char) sampTime[i]>>24; 
// 			data_time[4i+1] = (unsigned char) sampTime[i]>>16;
// 			data_time[4i+2] = (unsigned char) sampTime[i]>>8;
// 			data_time[4i+3] = (unsigned char) sampTime[i];
// 			data_mic[i] = mic[i];
// 	}
	
// 	for(int i = 4; i > 0; i--){
// 		data_env[i] = (unsigned char) envBuffer>>(i-1)*8;
// 	}


// }
