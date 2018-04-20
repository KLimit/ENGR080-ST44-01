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
#include<SpeakerControl.h>
#include<MasterBT.h>

#define mySerial Serial1

Printer printer;
Logger logger;

Adafruit_GPS GPS(&mySerial);
SensorGPS gps;
SensorIMU imu;

StateEstimator stateEst;
PControl pcont;
MotorDriver md;
SpeakerControl speaker;

MasterBT masterBT;

// GLOBALS (?)

double waypoints[] = {0, 10, 0, 0};

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

  // PARSONS COURTYARD
  const float origin_lat = 34.106111;
  const float origin_lon = 117.711667;
  stateEst.init(origin_lat, origin_lon);

  md.init();
  speaker.init();

  const int numWaypoints = 2;
  const int wayPointDim = 2;
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


}



void loop(){
  currentTime=millis();
  speaker.sendPulse();

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
    printer.printValue(9, masterBT.printBytesSent());
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
