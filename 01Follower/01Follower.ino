#include <Arduinoh>
#include <Pinouts.h>
#include <TimingOffsets.h>
#include <EnvADC.h>
#include<SensorGPS.h>
#include<SensorIMU.h>
#include<Adafruit_GPS.H>
#include<PControl.h>
#include<MotorDriver.h>
#include<StateEstimator.h>
#include<Printer.h>
#include<SendGPS.h>
#include<SlaveBT.h>

#define mySerial Serial1

Printer printer;
Logger logger;

MotorDriver md;

SensorGPS gps;
Adafruit_GPS GPS(&mySerial);
SensorIMU imu;
EnvADC env;

StateEstimator stateEst;
SlaveBT slaveBT;
SendGPS sendGPS;
PControl pcont;

int loopStartTime;
int currentTime;



void setup(){

  mySerial.begin(9600);
  BT_SERIAL.setTimeout(500);

  logger.include(&imu);
  logger.include(&gps);
  logger.include(&stateEst);
  logger.include(&md);
  logger.include(&env);

  logger.init();

  printer.init();
  imu.init();
  gps.init(&GPS);
  md.init();

  // const int numWaypoints = 2;
  // const int wayPointDim = 2;
  // const double waypoints[] = {0, 10, 0, 0};
  const double followDist = 5.0;
  pcont.init(NULL, NULL, NULL, followDist);



  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime   = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime       = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  gps.lastExecutionTime       = loopStartTime - LOOP_PERIOD + GPS_LOOP_OFFSET;
  stateEst.lastExecutionTim   = loopStartTime - LOOP_PERIOD + STATE_ESTIMATOR_LOOP_OFFSET;
  pcont.lastExecutionTime     = loopStartTime - LOOP_PERIOD + P_CONTROL_LOOP_OFFSET;
  logger.lastExecutionTime    = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;
  masterBT.lastExecutionTime  = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET; //Change this offset later

}



void loop(){
  currentTime=millis();

  if (currentTime-slaveBT.lastExecutionTime > LOOP_PERIOD) {
	slaveBT.lastExecutionTime = currentTime;
    slaveBT.receiveCoords();
  }

  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,adc.printSample());
    printer.printValue(1,logger.printState());
    printer.printValue(2,gps.printState());
    printer.printValue(3,stateEst.printState());
    printer.printValue(4,pcont.printWaypointUpdate());
    printer.printValue(5,pcont.printString());
    printer.printValue(6,md.printState());
    printer.printValue(7,imu.printRollPitchHeading());
    printer.printValue(8,imu.printAccels());
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-pcont.lastExecutionTime > LOOP_PERIOD ) {
    pcont.lastExecutionTime = currentTime;
    pcont.updateFollowerWaypoint();
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
