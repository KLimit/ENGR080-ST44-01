#ifndef __SLAVE_BT_H__
#define __SLAVE_BT_H__

#include <Arduino.h>
#include "SendGPS.h"
#include "Printer.h"

#define BT_SERIAL Serial3

class SlaveBT {
public:
  SlaveBT(void);

  void receiveCoords();
  unsigned long followTime;
  int kek = 0;
  int numBytes;
  byte buffAll[128]; // looking for lat float, separator char, lon float for 9 bytes total
  byte messageBuff[12];
  byte buffLat[4];
  byte buffLon[4];
  byte buffLeadTime[4];
  float lat;
  float lon;
 

  // unsigned long leaderTime = 0;

  int lastExecutionTime = -1;

  String printCoordinates();
  String printCoordinates2();
  String printCoordinates3();
  String printString = "";
  String printString2 = "";
  String printString3 = "";
private:
  int buffAllSize = 0;
  int loopCount = 0;
};
#endif
