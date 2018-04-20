#include "SendGPS.h"
#include "Printer.h"
extern Printer printer;
#include <Adafruit_GPS.h>

SendGPS::SendGPS(void)
: DataSource("leaderLat, leaderLon, leaderTime, followerTime", "float, float, ulong, ulong") {

  //HardwareSerial Uart = HardwareSerial();

  //Uart_p = &Uart;
}

void SendGPS::updateState(float lat, float lon, unsigned long leaderT, unsigned long followT)
{
  leaderLat = lat;
  leaderLon = lon;
  leaderTime = leaderT;
  followerTime = followT;

}

String SendGPS::printReceivedStates(){
  printString = "SendGPS Coordinates: \n";
  printString += "Lat: ";
  printString += String(leaderLat);
  printString += "Lon: ";
  printString += String(leaderLon);
  printString += "Leader Time: ";
  printString += String(leaderTime);
  printString += "Follower Time: ";
  printString += String(followerTime);
  return printString;
}

size_t SendGPS::writeDataBytes(unsigned char * buffer, size_t idx)
{
  //This section writes the floats in
  float * float_slot = (float *) (buffer + idx);
  float_slot[0] = leaderLat;
  float_slot[1] = leaderLon;
  idx += 2*sizeof(float);

  //This section writes the times in
  unsigned long * time_slot = (unsigned long*) (buffer + idx);
  time_slot[0] = leaderTime;
  time_slot[1] = followerTime;
  idx += 2*sizeof(unsigned long);
  return idx;
}
