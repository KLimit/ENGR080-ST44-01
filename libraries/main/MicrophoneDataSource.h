#ifndef __MICROPHONE_DATA_SOURCE_H__
#define __MICROPHONE_DATA_SOURCE_H__

#define LONGEST_CSV 100

#include <Arduino.h>

class MicrophoneDataSource {
public:
  // writes raw bytes of data to buffer, starting at index idx
  // returns the idx of the next vacant byte in the buffer
  virtual void writeDataBytes(unsigned long * timeBuffer, unsigned char * micBuffer, unsigned long envBuffer);

  // contains comma separated headings for each datavalue the dataSource reports
  const char * csvVarNames;
  const char * csvDataTypes;

protected:
  MicrophoneDataSource(const char varNames[], const char dataTypes[]) : csvVarNames(varNames), csvDataTypes(dataTypes) {}
};

#endif
