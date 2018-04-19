#ifndef __MASTER_BT_H__
#define __MASTER_BT_H__

#include <Arduino.h>
#include <SensorGPS.h>
#include <Adafruit_GPS.h>
#include "Printer.h"

#define BT_SERIAL Serial3


// NOTE: MUST HAVE THE BT_SERIAL.begin(38400) IN THE ARDUINO setup()

class MasterBT {
public:

  byte latBytes[4];
  byte lonBytes[4];
  byte timeBytes[4];
  int sentBytes;

  int lastExecutionTime = -1;
  // constructor
  MasterBT(void);

  void float2Bytes(float val, byte * bytes_array);
  void us2Bytes(unsigned long val, byte * bytes_array1);
  void sendCoords(SensorGPS * currentGPS);
  bool sendTest();

  String printBytesSent();
private:
  int testCounter = 0;
};

#endif
