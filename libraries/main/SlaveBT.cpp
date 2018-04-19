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
      if (inMessage && (i-startOfMessage < 13)) {
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

  lat = * (float*) &buffLat;
  lon = * (float*) &buffLon;
  unsigned long leaderTime = * (unsigned long*) &buffLeadTime;

  sendGPS.updateState(lat, lon, leaderTime, followTime); // Writes into sendGPS object.

  // Reset buffers after updating the state
  for(int i = 0; i <4; i++){
    buffLeadTime[i] = '0';
    buffLat[i] = '0';
    buffLon[i] = '0';
  }

  // PRINTING
  String buffString;
  printString2 = "Message Buff:  ";
  for (int i = 0; i < 12; i++) {
    buffString += String(char(messageBuff[i])) + " ";
  }
  printString2 += buffString;
  printString2 += ", LP: " + String(loopCount);
  delay(1);

  printString += "Time Bytes: ";
  for (int i = 0; i < 4; i++) {
    printString += String(char(buffLeadTime[i])) + " ";
     loopCount = loopCount + 1;
  }

  printString += ", LatBytes: ";
  for (int j = 0; j < 4; j++) {
    printString += String(char(buffLat[j])) + " ";
  }

  printString += ", Lon Bytes: ";
  for (int i = 0; i < 4; i++) {
    printString += String(char(buffLon[i])) + " ";
  }

  // const int n = snprintf(NULL, 0, "%lu", followTime);
  // char buf[n+1];
  // int c = snprintf(buf, n+1, "%lu", followTime);

  printString3 = "lat: " + String(lat,4) + ", lon: " + String(lon,4) + ", lead: " + String(leaderTime) + ", fol: " + String(followTime);

}

String SlaveBT::printCoordinates(void) {
  // COMMENTS BELOW ARE DEPRECATED; ALL THAT MATTERS IS printString
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

String SlaveBT::printCoordinates2(void) {
  return printString2;
}

String SlaveBT::printCoordinates3(void) {
  return printString3;
}
