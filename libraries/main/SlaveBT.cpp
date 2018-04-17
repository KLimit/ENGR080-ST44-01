#include "SlaveBT.h"

#define BT_SERIAL Serial3

extern SendGPS sendGPS;

SlaveBT::SlaveBT(){
  // no construction necessary
}

void receiveCoords(){
  numBytes = BT_SERIAL.available() + 1; //there may or may not be an imaginary 1 byte character at the start of the serial.
  if (numBytes > 1) {
    followTime = micros(); //Want to record the time immediately after the slave receives the bluetooth signal.
    buffAll = BT_SERIAL.readBytesUntil(47, buff, 13);
    BT_SERIAL.clear();
  }

  for(int i = 0; i <4; i++){
    buffLeadTime[i] = buffAll[i];
    buffLat[i] = buffAll[4+i];
    buffLon[i] = buffAll[8+i];
  }


  unsigned long leadTime = * (unsigned long*) &buffLeadTime;
  float lat = * (float*) &buffLat;
  float lon = * (float*) &buffLon;
  sendGPS.updateState(lat, lon, leadTime, followTime);
}
