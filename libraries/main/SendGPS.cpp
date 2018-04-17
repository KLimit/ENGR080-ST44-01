#include "SendGPS.h"
#include "Printer.h"
extern Printer printer;
#include <Adafruit_GPS.h>

SendGPS::SendGPS(void)
: DataSource("sentLat,sentLon, leaderTime, followTime","int32, int32, int32, int32") {

  //HardwareSerial Uart = HardwareSerial();

  //Uart_p = &Uart;
}

void SendGPS::updateState(float lat, float lon, float leaderTime, float followTime)
{
  receiveState.lat = lat;
  receiveState.lon = lon;
  receiveState.leaderTime = leaderTime;
  receiveState.followTime = followTime;

}

size_t SendGPS::writeDataBytes(unsigned char * buffer, size_t idx)
{
  float * float_slot = (float *) (buffer + idx);
  float_slot[0] = receiveState.lat;
  float_slot[1] = receiveState.lon;
  idx += 2*sizeof(float);
  return idx;
}
