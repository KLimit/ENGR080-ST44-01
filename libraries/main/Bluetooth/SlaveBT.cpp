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
    buffAll = BT_SERIAL.readBytesUntil(47, buff, 9);
    BT_SERIAL.clear();
  }

  for(int i = 0; i <4; i++){
    buffLat[i] = buffAll[i];
    buffLon[i] = buffAll[5+i];
  }


  float lat = * (float*) &buffLat;
  float lon = * (float*) &buffLon;
  sendGPS.updateState(lat, lon);
}
