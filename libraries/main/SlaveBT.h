#ifndef __SLAVE_BT_H__
#define __SLAVE_BT_H__

#include <Arduino.h>
#include "SendGPS.h"
#include "Printer.h"





class SlaveBT {
public:
  SlaveBT(void);

  void receiveCoords();
  float followTime;
  int kek = 0;
  int numBytes;
  byte buffAll[12]; // looking for lat float, separator char, lon float for 9 bytes total
  byte buffLat[4];
  byte buffLon[4];
  byte buffLeadTime[4];

  float leaderTime = 0;

  int lastExecutionTime = 0;

  String printCoordinates();
private:
};
#endif