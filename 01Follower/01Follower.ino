#include <Arduino.h>
#include <Pinouts.h>
#include <TimingOffsets.h>

#include<Printer.h>
#include<Logger.h>

#include<Adafruit_GPS.h>
#include<SensorGPS.h>
#include<SensorIMU.h>

#include<StateEstimator.h>
#include<PControl.h>
#include<MotorDriver.h>

#include<SlaveBT.h>
#include<SendGPS.h>

#include<EnvADC.h>

#define mySerial Serial1

Printer printer;
Logger logger;

Adafruit_GPS GPS(&mySerial);
SensorGPS gps;
SensorIMU imu;

StateEstimator stateEst;
PControl pcont;
MotorDriver md;

SlaveBT slaveBT;
SendGPS sendGPS;

EnvADC env;

// GLOBALS (?)

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
  logger.include(&sendGPS);
  logger.include(&env);
  logger.init();

  gps.init(&GPS);
  imu.init();
  md.init();

  const double followDist = 5.0;
  pcont.init(NULL, NULL, NULL, followDist);

  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime   = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime       = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  gps.lastExecutionTime       = loopStartTime - LOOP_PERIOD + GPS_LOOP_OFFSET;
  stateEst.lastExecutionTime  = loopStartTime - LOOP_PERIOD + STATE_ESTIMATOR_LOOP_OFFSET;
  pcont.lastExecutionTime     = loopStartTime - LOOP_PERIOD + P_CONTROL_LOOP_OFFSET;
  logger.lastExecutionTime    = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;
  slaveBT.lastExecutionTime  = loopStartTime - LOOP_PERIOD + BT_LOOP_OFFSET; //Change this offset later

}



void loop(){
  currentTime=millis();

  if (currentTime-slaveBT.lastExecutionTime > LOOP_PERIOD) {
    slaveBT.lastExecutionTime = currentTime;
    slaveBT.receiveCoords();
    //printer.printMessage(String(slaveBT.lat,4),1);
  }

  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    // printer.printValue(1,logger.printState());
    // printer.printValue(2,gps.printState());
    // printer.printValue(3,stateEst.printState());
    // printer.printValue(4,pcont.printWaypointUpdate());
    // printer.printValue(5,pcont.printString());
    // printer.printValue(6,md.printState());
    // printer.printValue(7,imu.printRollPitchHeading());
    // printer.printValue(8,imu.printAccels());
    printer.printMessage("002", 1);
    printer.printValue(2, slaveBT.printCoordinates());
    printer.printValue(3, slaveBT.printCoordinates2());
    printer.printValue(4, slaveBT.printCoordinates3());
    printer.printValue(5, sendGPS.printReceivedStates());
    printer.printToSerial();  // To stop printing, just comment this line out

  }

  if ( currentTime-pcont.lastExecutionTime > LOOP_PERIOD ) {
    pcont.lastExecutionTime = currentTime;
    pcont.updateFollowerWaypoint(sendGPS);
	  pcont.calculateFollowerControl(&stateEst.state);
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

  // uses the LED library to flash LED -- use this as a template for new libraries!

  if (currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }

}
