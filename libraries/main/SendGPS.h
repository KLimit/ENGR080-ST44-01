#ifndef __SEND_GPS_H__
#define __SEND_GPS_H__

// #define GPS_READ_INTERVAL 3


#include <Arduino.h>
//#include <TinyGPS.h>
#include "DataSource.h"
#include "Pinouts.h"
//#define GPSECHO  true
#include <Adafruit_GPS.h>

<<<<<<< HEAD

=======
typedef struct {
  float lat;
  float lon;
  unsigned long leaderTime;
  unsigned long followTime;
} receive_state_t;
>>>>>>> c7e6f6446ec343b1bcdba23e4129dd411952899c

class SendGPS : public DataSource {
public:
	SendGPS(void);

<<<<<<< HEAD
  float lat = 0
  float lon = 0
  float leaderTime;
  float followTime;
  void updateState(float lat, float lon, float leaderTime, float followTime);
=======
  receive_state_t receiveState;
  receiveState.lat = 0;
  receiveState.lon = 0;
  void updateState(float lat, float lon, unsigned long leaderTime, unsigned long followTime);
>>>>>>> c7e6f6446ec343b1bcdba23e4129dd411952899c




  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;



};

#endif
