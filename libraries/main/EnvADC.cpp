#include "EnvADC.h"
#include <math.h>
#include "Printer.h"
#include "Pinouts.h"
extern Printer printer;

EnvADC::EnvADC(void)
  : DataSource("EnvelopeTime", "int"){

}



void EnvADC::updateSample(){
  interruptTime = micros();
}
// String EnvADC::printSample(void)
// {
//   String printString = "ADC:";
//   for (int i=0; i<NUM_PINS; i++) {
//     printString += " ";
//     printString += String(interruptTime[i]);
//   }
//   return printString; //printer.printValue(0, printString);
// }

size_t EnvADC::writeDataBytes(unsigned char* buffer, size_t idx)
{
  int* data_slot = (int*) &buffer[idx];
  data_slot[0] = interruptTime;
  return idx + sizeof(int);
}

// void EnvADC::writeDataBytes(unsigned char * timeBuffer, unsigned char * micBuffer, unsigned char * envBuffer)
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
