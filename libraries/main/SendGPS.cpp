#include "SendGPS.h"
#include "Printer.h"
extern Printer printer;
#include <Adafruit_GPS.h>

SendGPS::SendGPS(void)
: DataSource("sentLat,sentLon, leaderTime, followTime","float, float, ulong, ulong") {

  //HardwareSerial Uart = HardwareSerial();

  //Uart_p = &Uart;
}

void SendGPS::updateState(float lat, float lon, unsigned long leaderTime, unsigned long followTime)
{
  lat = lat;
  lon = lon;
  leaderTime = leaderTime;
  followTime = followTime;

}

size_t SendGPS::writeDataBytes(unsigned char * buffer, size_t idx)
{
  //This section writes the floats in
  float * float_slot = (float *) (buffer + idx);
  float_slot[0] = lat;
  float_slot[1] = lon;
  idx += 2*sizeof(float);

  //This section writes the times in
  unsigned long * time_slot = (unsigned long*) (buffer + idx);
  time_slot[0] = leaderTime;
  time_slot[1] = followTime;
  idx += 2*sizeof(unsigned long);
  return idx;
}
