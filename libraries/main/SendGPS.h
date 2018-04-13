#ifndef __SEND_GPS_H__
#define __SEND_GPS_H__

// #define GPS_READ_INTERVAL 3


#include <Arduino.h>
//#include <TinyGPS.h>
#include "DataSource.h"
#include "Pinouts.h"
//#define GPSECHO  true
#include <Adafruit_GPS.h>

typedef struct {
  float lat;
  float lon;
} receive_state_t;

class SendGPS : public DataSource {
public:
	SendGPS(void);

  receive_state_t receiveState;
  receiveState.lat = 0;
  receiveState.lon = 0;
  void updateState(float lat, float lon);




  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;



};

#endif
