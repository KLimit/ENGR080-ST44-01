#include "MasterBT.h"

#define BT_SERIAL Serial3
extern SensorGPS gps;


MasterBT::MasterBT(){
  // no construction necessary
}

void MasterBT::float2Bytes(float floatVal, byte * bytes_array){
  // make union of shared memory space
  union {
    float float_var;
    byte temp_array[4];
  } u;
  // overwrite the union's bytes with a float variable
  u.float_var = floatVal;
  // assigns bytes to the input array
  memcpy(byte_array, u.temp_array, 4);
}

void MasterBT::sendCoords(SensorGPS * currentGPS) {
  float2bytes(micros(), &timeBytes[0]);  // send the time sent first
  float2bytes(currentGPS->state.lat, &latBytes[0]);
  float2bytes(currentGPS->state.lon, &lonBytes[0]);
  sentBytes += BT_SERIAL.write(timeBytes, 4)
  sentBytes = BT_SERIAL.write(latBytes, 4);
  sentBytes += BT_SERIAL.write(lonBytes, 4);
  sentBytes += BT_SERIAL.write(47);

  BT_SERIAL.flush();
}
