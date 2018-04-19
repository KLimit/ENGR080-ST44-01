#include "SlaveBT.h"

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

    buffAllSize = BT_SERIAL.readBytesUntil(47, buffAll, 50);
    bool inMessage = false;
    int startOfMessage = 0;
    for (int i = 0; i < buffAllSize; i++) {
      if (inMessage && (i-startOfMessage <12)) {
        messageBuff[i - startOfMessage] = buffAll[i];
      }
      if (buffAll[i] == '?') {
        inMessage = true;
        startOfMessage = i + 1;
      }
    }
    inMessage = false;
    BT_SERIAL.clear();
  }

  for(int i = 0; i <4; i++){
    buffLeadTime[i] = messageBuff[i];
    buffLat[i] = messageBuff[4+i];
    buffLon[i] = messageBuff[8+i];
  }

  float lat = * (float*) &buffLat;
  float lon = * (float*) &buffLon;
  unsigned long leaderTime = * (unsigned long*) &buffLeadTime;

// PRINTING
  String buffString;
  printString2 = "Message Buff:  ";
  for (int i = 0; i < 12; i++) {
    buffString += String(char(messageBuff[i])) + " ";
  }
  printString2 += buffString;

  printString += "\nTime Bytes: ";
  for (int i = 0; i < 4; i++) {
    printString += String(char(buffLeadTime[i])) + " ";
  }

  printString += "\nLatBytes: ";
  for (int j = 0; j < 4; j++) {
    printString += String(char(buffLat[j])) + " ";
  }

  printString += "\nLon Bytes: ";
  for (int i = 0; i < 4; i++) {
    printString += String(char(buffLon[i])) + " ";
  }

  //printString += "\nlat: " + String(lat) + "\nlon: " + String(lon) + "\nleaderTime: " + String(leaderTime) + "\nfollowerTime: " + String(followTime);

  sendGPS.updateState(lat, lon, leaderTime, followTime); //Writes into sendGPS object.
}

String SlaveBT::printCoordinates(void)
{

  // printString += buffString;
  // String printString = "Lat: ";
  // printString += String(sendGPS.lat);
  // printString += "\nLon: ";
  // printString += String(sendGPS.lon);
  // printString += "\nFollower Time: ";
  // printString += String(sendGPS.followTime);
  // printString += "\nLeader Time ";
  // printString += String(sendGPS.leaderTime);

  return printString; //printer.printValue(0, printString);
}

String SlaveBT::printCoordinates2(void)
{

  return printString2; //printer.printValue(0, printString);
}