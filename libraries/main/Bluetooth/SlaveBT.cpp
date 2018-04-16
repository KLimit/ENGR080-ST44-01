#include "SlaveBT.h"

#define BT_SERIAL Serial3

extern SendGPS sendGPS;

SlaveBT::SlaveBT(){
  // no construction necessary
}

void receiveCoords(){
  int startTime = micros();
  numBytes = BT_SERIAL.available() + 1; //there may or may not be an imaginary 1 byte character at the start of the serial.
  if (numBytes > 1) {
    followTime = micros(); //Want to record the time immediately after the slave receives the bluetooth signal.
    buffAll = BT_SERIAL.readBytesUntil(47, buff, 13);
    BT_SERIAL.clear();
  }

  for(int i = 0; i <4; i++){
    buffLat[i] = buffAll[i];
    buffLon[i] = buffAll[5+i];
    buffLeadTime[i] = buffAll[9+i];
  }


  float lat = * (float*) &buffLat;
  float lon = * (float*) &buffLon;
  float leadTime = * (float*) &buffLeadTime;
  sendGPS.updateState(lat, lon, leadTime, followTime);
}
