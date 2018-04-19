#include "MasterBT.h"

extern SensorGPS gps;
extern Printer printer;



MasterBT::MasterBT(){
  // none needed
}

void MasterBT::float2Bytes(float floatVal, byte* bytes_array){
  // make union of shared memory space
  union {
    float float_var;
    byte temp_array[4];
  } u;
  // overwrite the union's bytes with a float variable
  u.float_var = floatVal;
  // assigns bytes to the input array
  memcpy(bytes_array, u.temp_array, 4);
}

void MasterBT::sendCoords(SensorGPS * currentGPS) {
  float2Bytes(micros(), &timeBytes[0]);  // send the time sent first
  float2Bytes(currentGPS->state.lat, &latBytes[0]);
  float2Bytes(currentGPS->state.lon, &lonBytes[0]);
  sentBytes = BT_SERIAL.write(63);  // ASCII for '?'
  sentBytes += BT_SERIAL.write(timeBytes, 4);
  sentBytes += BT_SERIAL.write(latBytes, 4);
  sentBytes += BT_SERIAL.write(lonBytes, 4);
  sentBytes += BT_SERIAL.write(47);  // ASCII for '/'
  BT_SERIAL.flush();
}

bool MasterBT::sendTest() {
  float2Bytes(micros(), &timeBytes[0]);
  float floatLatBytes = 12.3456; //testing
  float floatLonBytes = 45.6789;//testing
  float2Bytes(floatLatBytes, &latBytes[0]);
  float2Bytes(floatLonBytes, &lonBytes[0]);
  sentBytes = BT_SERIAL.write(63);  // ASCII for '?'
  sentBytes += BT_SERIAL.write(timeBytes, 4);
  sentBytes += BT_SERIAL.write(latBytes, 4);
  sentBytes += BT_SERIAL.write(lonBytes, 4);
  sentBytes += BT_SERIAL.write(47);  // ASCII for '/'
  if (sentBytes == 13) {
    return true;
  } else {
    return false;
  }
  BT_SERIAL.flush();
}



String MasterBT::printBytesSent(void)
{
  String printString = "Bytes sent:";
  printString += String(sentBytes);

  return printString; //printer.printValue(0, printString);
}
