#include <Arduino.h>
#include <Pinouts.h>
#include <TimingOffsets.h>
#include <Wire.h>

#include<Printer.h>
#include<Logger.h>

#include<Adafruit_GPS.h>
#include<SensorGPS.h>
#include<SensorIMU.h>

#include<StateEstimator.h>
#include<PControl.h>
#include<MotorDriver.h>
#include<SpeakerControl.h>
#include<MasterBT.h>
#include<SlaveBT.h>
#define mySerial Serial1

Printer printer;
Logger logger;

SensorGPS gps;
Adafruit_GPS GPS(&mySerial);
SensorIMU imu;

StateEstimator stateEst;
PControl pcont;
MotorDriver md;
SpeakerControl speaker;

MasterBT masterBT;
SlaveBT slaveBT;

// GLOBALS (?)

double waypoints[] = {0, 0, 1.2, -8.2, -5.37, -15.48, -13.29, -3.88, -2.72, 0};

int loopStartTime;
int currentTime;



void setup(){

  mySerial.begin(9600);
  BT_SERIAL.begin(38400);
  printer.init();

  logger.include(&gps);
  logger.include(&imu);
  logger.include(&stateEst);
  logger.include(&md);
  logger.include(&speaker);
  logger.init();

  gps.init(&GPS);
  imu.init();

  //GPS COORDS
  gps.read(&GPS);
  while(gps.state.lat == 0){
  
  printer.printMessage(String(gps.state.lat,10),10);
  gps.read(&GPS);
  } 
  printer.printMessage("Final lat: " + String(gps.state.lat), 10);
  const float origin_lat = gps.state.lat;
  const float origin_lon = gps.state.lon;
  gps.originLat = origin_lat;
  gps.originLon = origin_lon;
  stateEst.init(origin_lat, origin_lon);
  
  

  md.init();
  speaker.init();

  const int numWaypoints = 5;
  const int wayPointDim = 5;
  const double followDist = -1.0;
  pcont.init(numWaypoints, wayPointDim, waypoints, followDist);

  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime   = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime       = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  gps.lastExecutionTime       = loopStartTime - LOOP_PERIOD + GPS_LOOP_OFFSET;
  stateEst.lastExecutionTime   = loopStartTime - LOOP_PERIOD + STATE_ESTIMATOR_LOOP_OFFSET;
  pcont.lastExecutionTime     = loopStartTime - LOOP_PERIOD + P_CONTROL_LOOP_OFFSET;
  masterBT.lastExecutionTime  = loopStartTime - LOOP_PERIOD + SEND_DATA_OFFSET;
  logger.lastExecutionTime    = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;
  slaveBT.lastExecutionTime = loopStartTime - LOOP_PERIOD + BT_LOOP_OFFSET;
  
}



void loop(){
  currentTime=millis();
  if (currentTime-slaveBT.lastExecutionTime > LOOP_PERIOD) {
    slaveBT.lastExecutionTime = currentTime;
    speaker.sendPulse();
  }
  

  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(1,logger.printState());
    printer.printValue(2,gps.printState());
    printer.printValue(3,stateEst.printState());
    printer.printValue(4,pcont.printWaypointUpdate());
    printer.printValue(5,pcont.printLeaderString());
    printer.printValue(6,md.printState());
    printer.printValue(7,imu.printRollPitchHeading());
    printer.printValue(8,imu.printAccels());
    // printer.printValue(9, masterBT.printBytesSent());

    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-pcont.lastExecutionTime > LOOP_PERIOD ) {
    pcont.lastExecutionTime = currentTime;
    pcont.calculateLeaderControl(&stateEst.state);
    md.driveForward(pcont.uL,pcont.uR);
      
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read();     // blocking I2C calls
  }

  if (true){//(gps.loopTime(loopStartTime)) {
    gps.lastExecutionTime = currentTime;
    gps.read(&GPS); // blocking UART calls
  }



  if ( currentTime-stateEst.lastExecutionTime > LOOP_PERIOD ) {
    stateEst.lastExecutionTime = currentTime;
    stateEst.updateState(&imu.state, &gps.state);
  }

  // Bluetooth timer
  // printer.printMessage("bluetooth? " + String(currentTime-masterBT.lastExecutionTime > LOOP_PERIOD), 1);

  //The runtime of this code is extremely short
  if (currentTime-masterBT.lastExecutionTime > LOOP_PERIOD) {
    masterBT.lastExecutionTime = currentTime;
    masterBT.sendCoords(&gps);
    
  }



  if (currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }

}
