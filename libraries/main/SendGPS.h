#ifndef __SEND_GPS_H__
#define __SEND_GPS_H__

// #define GPS_READ_INTERVAL 3

#include <Arduino.h>
//#include <TinyGPS.h>
#include "DataSource.h"
#include "Pinouts.h"
//#define GPSECHO  true
#include <Adafruit_GPS.h>

class SendGPS : public DataSource {
public:
	SendGPS(void);

  float leaderLat = 0;
  float leaderLon = 0;
  unsigned long leaderTime;
  unsigned long  followerTime;

  void updateState(float lat, float lon, unsigned long leaderT, unsigned long followT);

  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;

};

#endif
