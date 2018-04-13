#include <Arduino.h>
#include <Pinouts.h>
#include <Wire.h>
#include <analogComp.h>
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
#define followDist 2.0F

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


//loop start recorder
int loopStartTime;

int currentTime;



void setup() {
  pinMode(A16, INPUT);
  analogWriteResolution(12);
  analogWrite(A14, 0.18*4096/3.3); //Sets the comparator voltage
  analogComparator.setOn(3,2);
  analogcomparator.enableInterrupt(pulse, RISING);
  gps.init(&GPS);
  imu.init();
  // mySerial.begin(9600);
  md.init();

  // x0, y0, x1, y1, ...

  pcont.init(NULL, NULL, NULL, followDist);

  const float origin_lat = 34.106465;
  const float origin_lon = -117.712488;


//TIMING STUFF: We may wish to change this later.
 printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime         = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime             = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  gps.lastExecutionTime             = loopStartTime - LOOP_PERIOD + GPS_LOOP_OFFSET;
  adc.lastExecutionTime             = loopStartTime - LOOP_PERIOD + ADC_LOOP_OFFSET;
  state_estimator.lastExecutionTime = loopStartTime - LOOP_PERIOD + STATE_ESTIMATOR_LOOP_OFFSET;
  pcontrol.lastExecutionTime        = loopStartTime - LOOP_PERIOD + P_CONTROL_LOOP_OFFSET;
  logger.lastExecutionTime          = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;


}

void loop(){
  //Very rudimentary control scheme:
  pcont.calculateControl(&stateEst.state);
  md.driveForward(pcont.ul, pcont.ur); //This line is subject to change.

  imu.read();
  gps.read(&GPS);
  stateEst.updateState(&imu.state, &gps.state);
  slaveBT.receiveCoords();


  //Convert the received coordinates into x-y coordinates
  float x = stateEst

}


void pulse(){
  st = 1;
  envADC.updateSample();

}