#include "SendGPS.h"
#include "Printer.h"
extern Printer printer;
#include <Adafruit_GPS.h>

SendGPS::SendGPS(void)
: DataSource("sentLat,sentLon, leaderTime, followTime","int32, int32, int32, int32") {

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
  float * float_slot = (float *) (buffer + idx);
  float_slot[0] = lat;
  float_slot[1] = lon;
  float_slot[2] = leaderTime;
  float_slot[3] = followTime;
  idx += 4*sizeof(float); //floats and unsigned longs are the same size
  return idx;
}
