#include <Arduino.h>
#include <Pinouts.h>
#include <Wire.h>

#include<SensorGPS.h>
#include<SensorIMU.h>
#include<Adafruit_GPS.h>
#include<PControl.h>
#include<MotorDriver.h>
#include <StateEstimator.h>
#include <Printer.h>
#include <EnvADC.h>
#include <SlaveBT.h>
#include <SendGPS.h>

#define mySerial Serial1
#define BT_SERIAL Serial3
#define notLeader false

SensorGPS gps;
SensorIMU imu;
Adafruit_GPS GPS(&mySerial);
PControl pcont;
MotorDriver md;
StateEstimator stateEst;
EnvADC envADC;
Printer printer;
SlaveBT slaveBT;
SendGPS sendGPS;




void setup() {
  gps.init(&GPS);
  imu.init();
  // mySerial.begin(9600);
  md.init();

  // x0, y0, x1, y1, ...

  pcontrol.init(NULL, NULL, NULL, notLeader);

  const float origin_lat = 34.106465;
  const float origin_lon = -117.712488;



}

void loop(){
  //Very rudimentary control scheme:
  PControl.calculateControl(&stateEst.state);
  md.driveForward(PControl.ul, PControl.ur); //This line is subject to change.

  envADC.updateSample();
  imu.read();
  gps.read(&GPS);
  stateEst.updateState(&imu.state, &gps.state);
  slaveBT.receiveCoords();


  //Convert the received coordinates into x-y coordinates
  float x = stateEst

}
