#include "MicLogger.h"
#include <stdio.h>
#include "Printer.h"
#include "MicrophoneADC.h"
#include "EnvADC.h"
extern Printer printer;
extern MicrophoneADC mic;  // assumes that there's an object in memory named mic
// String message;
extern EnvADC env;

MicLogger::MicLogger(void){
}


// void MicLogger::includeInt(int * target)
// void MicLogger::includeBool(bool * target)
// void MicLogger::includeFloat(float * target)

void MicLogger::padding(int number, byte width, String & str) {
	int currentMax = 10;
	for (byte i = 1; i < width; i++) {
		if (number < currentMax) {
			str.concat("0");
		}
		currentMax *= 10;
	}
	str.concat(number);
}

void MicLogger::init(void) {
	pinMode(MIC_PIN, INPUT);
	Serial.print("Initializing SD Card... ");
  if (!SD.begin()) {
    Serial.println("failed!");
    return;
  }
  Serial.println("done!");

	unsigned int number = 0;
	String numstr = "";
	\(number, 3, numstr);
	String finalname = LOG_FILENAME_BASE + numstr + ".bin";
	finalname.toCharArray(logfilename, LOG_FILENAME_BUFFERLEN);

	while(SD.exists(logfilename)) {
		number++;
		numstr = "";
		padding(number, 3, numstr);
		finalname = LOG_FILENAME_BASE + numstr + ".bin";
		finalname.toCharArray(logfilename, LOG_FILENAME_BUFFERLEN);
	}

	finalname = HEADINGS_FILENAME_BASE + numstr + ".txt";
	finalname.toCharArray(headingfilename, LOG_FILENAME_BUFFERLEN);

	// message = "MicLogger: Using log file name " + String(logfilename);
	// printer.printMessage(message,30);

	String headingStr = "time, amplitude, envTime";
	String dataTypeStr = "ulong, uint8, uint8";
	headingStr += "\n"+dataTypeStr;

	file = SD.open(headingfilename, FILE_WRITE);

	// if the file exists, use it:
  if (file) {
    file.println(headingStr);
    file.close();
	}

	// printer.printMessage("Creating log file",10);
	file = SD.open(logfilename, FILE_WRITE);
	if(!file) {
		// message = "MicLogger: error creating " + String(logfilename);
		// printer.printMessage(message,0);
	} else {
		file.close();
	}

	// if exiting without error
	keepLogging = true;
}

bool MicLogger::log(void){
	// record data from sources

	unsigned char buffer[BUFFER_BLOCK_COUNT*BYTES_PER_BLOCK];


	//sources[0] should be a microphone object
	mic.writeDataBytes(&buffer[0]);


	// // write data to SD
	// if (writtenBlocks >= FILE_BLOCK_COUNT) {
	// 	printer.printMessage("Current file size limit reached. Change FILE_BLOCK_COUNT to fix. Stopping logging for now.",0);
	// 	keepLogging = false;
	// }

	file = SD.open(logfilename, FILE_WRITE);
	if (file) {
		// write time
		file.write(&buffer[0], BUFFER_BLOCK_COUNT*BYTES_PER_BLOCK);

	}
	file.close();
	Serial.println("Done logging!");
	return true;
}

bool MicLogger::envLog(void){
	unsigned char * buffer;

	env.writeDataBytes(&buffer[0]);
	file = SD.open(logfilename, FILE_WRITE);
	if (file) {
		// write time
		file.write(&buffer[0], 4); //4 bytes in an int

	}
	file.close();
	return true;

}
// String MicLogger::printState(void){
// 	String printString = "MicLogger: ";
// 	if(keepLogging) {
// 		printString += "Just logged buffer " + String(writtenBlocks) + " to SD.";
// 	} else {
// 		printString += "LOGGING ERROR, LOGGING HAS STOPPED";
// 	}
//
// 	return printString;
// }
