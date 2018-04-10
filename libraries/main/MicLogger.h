#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <stdio.H>

#define LOG_FILENAME_BASE "log"
#define LOG_FILENAME_BUFFERLEN 20
#define HEADINGS_FILENAME_BASE "inf"
// buffered logging
// number of 512B blocks in the log file
// #define FILE_BLOCK_COUNT 8192 // should last over 10 min i DON'T CARE ABOUT THE BYTES WRITTEN TO THE SD I HAVE INFINITE STORAGE
#define BYTES_PER_BLOCK 6  //number of bytes in one mic_sample_t datatype

// number of blocks in the buffer
#define BUFFER_BLOCK_COUNT 512
#define MAX_NUM_DATASOURCES 1

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
// #include "MicrophoneDataSource.h"
#include "Pinouts.h"


class MicLogger {
public:
	MicLogger(void);

    // include all dataSources before running init
	// void include(MicrophoneDataSource * source_p);

    // run after all MicrophonedataSources have been registered
	void init(void);

	// records all data at the time it's called to the SD
	bool log(void);

	String printState(void);
	int lastExecutionTime = -1;
	bool keepLogging = false;

private:
	void padding(int number, byte width, String & str);

	// MicrophoneDataSource* sources[MAX_NUM_DATASOURCES];
	unsigned int num_datasources;
	char logfilename[LOG_FILENAME_BUFFERLEN];
	char headingfilename[LOG_FILENAME_BUFFERLEN];
	File file;

	// uint32_t writtenBlocks = 0;
};
#endif
