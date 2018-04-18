#ifndef __MASTER_BT_H__
#define __MASTER_BT_H__

#include <Arduino.h>
#include <SensorGPS.h>
#include <Adafruit_GPS.h>
#include "Printer.h"






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
  void sendCoords(SensorGPS * currentGPS);
  void sendTest();

  String printBytesSent();

private:

};


#endif
