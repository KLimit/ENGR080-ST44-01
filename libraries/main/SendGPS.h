#ifndef __SEND_GPS_H__
#define __SEND_GPS_H__

// #define GPS_READ_INTERVAL 3

// #define RADIUS_OF_EARTH_M 6371000

#include <Arduino.h>
//#include <TinyGPS.h>
#include "DataSource.h"
#include "Pinouts.h"
//#define GPSECHO  true
#include <Adafruit_GPS.h>

typedef struct {
  float lat;
  float lon;
  uint32_t age;
  uint16_t hdop;
  uint8_t num_sat;
} gps_state_t;

class SendGPS : public DataSource {
public:
	SendGPS(void);
  

  void updateState(float lat, float lon);
  


  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;


  
};

#endif
