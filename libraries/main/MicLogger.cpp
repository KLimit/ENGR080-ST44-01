#include "MicLogger.h"
#include <stdio.h>
#include "Printer.h"
extern Printer printer;
String message;

MicLogger::MicLogger(void)
	: num_datasources(0)
{
}

void MicLogger::include(MicrophoneDataSource * source_p) {
	sources[num_datasources] = source_p;
	++num_datasources;
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
	Serial.print("Initializing SD Card... ");
  if (!SD.begin()) {
    Serial.println("failed!");
    return;
  }
  Serial.println("done!");

	unsigned int number = 0;
	String numstr = "";
	padding(number, 3, numstr);
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

	message = "MicLogger: Using log file name " + String(logfilename);
	printer.printMessage(message,30);

	String headingStr = sources[0]->csvVarNames;
	String dataTypeStr = sources[0]->csvDataTypes;
	for(size_t i = 1; i < num_datasources; ++i) {
		headingStr += ",";
		headingStr += sources[i]->csvVarNames;
		dataTypeStr += ",";
		dataTypeStr += sources[i]->csvDataTypes;
	}
	headingStr += "\n"+dataTypeStr;

	file = SD.open(headingfilename, FILE_WRITE);

	// if the file exists, use it:
  if (file) {
    file.println(headingStr);
    file.close();
	}

	printer.printMessage("Creating log file",10);
	file = SD.open(logfilename, FILE_WRITE);
	if(!file) {
		message = "MicLogger: error creating " + String(logfilename);
		printer.printMessage(message,0);
	} else {
		file.close();
	}

	// if exiting without error
	keepLogging = true;
}

bool MicLogger::log(void){
	// record data from sources

	unsigned long timeBuffer[BYTES_PER_BLOCK];
	unsigned char micBuffer[BYTES_PER_BLOCK];
	unsigned long envTime = 0;


	//sources[0] should be a microphone object
	sources[0]->writeDataBytes(timeBuffer, micBuffer, envTime);


	// // write data to SD
	// if (writtenBlocks >= FILE_BLOCK_COUNT) {
	// 	printer.printMessage("Current file size limit reached. Change FILE_BLOCK_COUNT to fix. Stopping logging for now.",0);
	// 	keepLogging = false;
	// }

	file = SD.open(logfilename, FILE_WRITE);
	if (file) {
		// write time
		file.write(&timeBuffer,BYTES_PER_BLOCK*4);  // casted as int, so 4 bytes * number of samples
		file.write(&micBuffer,BYTES_PER_BLOCK*4); //casted as int, so 4 bytes * number of samples
		file.write((int) &envBuffer,4)  // 4 bytes total for one unsigned long

	}
	file.close();
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
