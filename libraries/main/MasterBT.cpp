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
  us2Bytes(micros(), &timeBytes[0]);  // send the time sent first
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
  //unsigned long x = 123456789; //sending: 0-11000101-1 0-10011110-1 0-11010111-1 0-00110010-1
  //byte y[4] = {32,16,8,4}; // sends: 0-00000100-1 0-00001000-1 0-00010000-1 0-00100000-1

  us2Bytes(micros(), &timeBytes[0]);

  float floatLatBytes;
  float floatLonBytes;
  if (testCounter % 2 == 0) {
    floatLatBytes = 45.6789;
    floatLonBytes = 12.3456;
  }
  else{
    floatLatBytes = 12.3456;
    floatLonBytes = 45.6789;
  }
  testCounter ++;

  float2Bytes(floatLatBytes, &latBytes[0]);
  float2Bytes(floatLonBytes, &lonBytes[0]);
  //BT_SERIAL.write(y,4);
  sentBytes = BT_SERIAL.write(63);  // ASCII for '?' 0-11111100-1
  sentBytes += BT_SERIAL.write(timeBytes, 4); // 0-10101000-1 0-10110011-1 0-11011010-1 0-11100000-1
  // sentBytes = BT_SERIAL.write(63);
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

void MasterBT::us2Bytes(unsigned long usVal, byte* bytes_array1){
  // make union of shared memory space
  union {
    unsigned long us_var;
    byte temp_array1[4];
  } u;
  // overwrite the union's bytes with a float variable
  u.us_var = usVal;
  // assigns bytes to the input array
  memcpy(bytes_array1, u.temp_array1, 4);
}
