#include <Arduino.h>
#include <Pinouts.h>
#include <Wire.h>

#include<SensorGPS.h>
#include<SensorIMU.h>
#include<Adafruit_GPS.h>
#include<PControl.h>
#include<MotorDriver.h>
#include <StateEstimator.h>

#define mySerial Serial1

SensorGPS gps;
SensorIMU imu;
Adafruit_GPS GPS(&mySerial);
PControl pcont;
MotorDriver md;
StateEstimator stateEst;



void setup() {
  gps.init(&GPS);
  imu.init();
  // mySerial.begin(9600);
  md.init();

  // x0, y0, x1, y1, ...
  const int waypointDimensions = 2;
  double waypoints[] = {0, 10, 0, 0};
  const int numberWaypoints = 2; // The length of waypoints divided by dimension
  pcontrol.init(numberWaypoints, waypointDimensions, waypoints);

  const float origin_lat = 34.106465;
  const float origin_lon = -117.712488;



}
