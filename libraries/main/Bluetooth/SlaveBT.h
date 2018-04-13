#ifndef __SLAVE_BT_H__
#define __SLAVE_BT_H__

#include <Arduino.h>
#include "SendGPS.h"





class SlaveBT {
public:
  SlaveBT(void);

  void recieveCoords();

  int kek = 0;
  int numBytes;
  byte buffAll[8]; // looking for lat float, separator char, lon float for 9 bytes total
  byte buffLat[4];
  byte buffLon[4];

private:
}
