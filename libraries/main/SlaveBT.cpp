#include "SlaveBT.h"

#define BT_SERIAL Serial3

extern SendGPS sendGPS;
extern Printer printer;

SlaveBT::SlaveBT(){
  // no construction necessary
}

void SlaveBT::receiveCoords(){
  int startTime = micros();
  numBytes = BT_SERIAL.available() + 1; //there may or may not be an imaginary 1 byte character at the start of the serial.
  if (numBytes > 1) {
    followTime = micros(); //Want to record the time immediately after the slave receives the bluetooth signal.
    BT_SERIAL.readBytesUntil(47, buffAll, 13);
    BT_SERIAL.clear();
  }

  for(int i = 0; i <4; i++){
    buffLat[i] = buffAll[i];
    buffLon[i] = buffAll[5+i];
    buffLeadTime[i] = buffAll[9+i];
  }


  float lat = * (float*) &buffLat;
  float lon = * (float*) &buffLon;
  leaderTime = * (float*) &buffLeadTime;

  sendGPS.updateState(lat, lon, leaderTime, followTime); //Writes into sendGPS object.
}

String SlaveBT::printCoordinates(void)
{
  // String printString = "Lat:";
  // printString += String(sendGPS.lat);
  // printString += "Lon:";
  // printString += String(sendGPS.lon);
  // printString += "Follower Time:";
  // printString += String(sendGPS.followTime);
  // printString += "Leader Time";
  // printString += String(sendGPS.leaderTime);

  String printString = "Leader Time:";
  printString += String(leaderTime);

  return printString; //printer.printValue(0, printString);
}