/********
Default E80 Lab 01 
Current Author: Christopher McElroy (cmcelroy@g.hmc.edu) '19 (contributed in 2017)
Previous Contributors:  Josephine Wong (jowong@hmc.edu) '18 (contributed in 2016)
                        Apoorva Sharma (asharma@hmc.edu) '17 (contributed in 2016)
*/

#include <Arduino.h>
#include <Wire.h>
#include <Pinouts.h>
#include <TimingOffsets.h>

#include <SensorGPS.h>
#include <SensorIMU.h>
#include <StateEstimator.h>
#include <Adafruit_GPS.h>
#include <ADCSampler.h>
#include <MotorDriver.h>
#include <Logger.h>
#include <Printer.h>
#include <PControl.h>
#define mySerial Serial1
#define BTSerial Serial3
#include <LED.h>  // A template of a data soruce library
#include <SendGPS.h>

/////////////////////////* Global Variables *////////////////////////

MotorDriver motor_driver;
StateEstimator state_estimator;
PControl pcontrol;
SensorGPS gps;
Adafruit_GPS GPS(&mySerial);  // FIX THIS
ADCSampler adc;
SensorIMU imu;
Logger logger;
Printer printer;
LED led;
SendGPS sendGPS;

// loop start recorder
int loopStartTime;
int currentTime;
int current_way_point = 0;

////////////////////////* Setup *////////////////////////////////
int x = 0;
char y = 0;
int Listen = 0;
int kek = 0;
int numBytes;
int gpsSwitch = 0;
int latSwitch = 0;
byte buff[9];
byte buff2[4];
void setup() {
  BTSerial.begin(38400);
  BTSerial.clear();
  pinMode(2,INPUT_PULLUP);
  logger.include(&imu);
  logger.include(&gps);
  logger.include(&state_estimator);
  logger.include(&motor_driver);
  logger.include(&adc);
  logger.init();

  printer.init();
  imu.init();
  mySerial.begin(9600);
  gps.init(&GPS);
  motor_driver.init();
  led.init();

  const int number_of_waypoints = 2;
  const int waypoint_dimensions = 2;       // waypoints have two pieces of information, x then y.
  double waypoints [] = { 0, 10, 0, 0 };   // listed as x0,y0,x1,y1, ... etc.
  pcontrol.init(number_of_waypoints, waypoint_dimensions, waypoints);
  
  const float origin_lat = 34.106465;
  const float origin_lon = -117.712488;
  state_estimator.init(origin_lat, origin_lon); 

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



//////////////////////////////* Loop */////////////////////////

void loop() {
  currentTime=millis();
  
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,adc.printSample());
    printer.printValue(1,logger.printState());
    printer.printValue(2,gps.printState());   
    printer.printValue(3,state_estimator.printState());     
    printer.printValue(4,pcontrol.printWaypointUpdate());
    printer.printValue(5,pcontrol.printString());
    printer.printValue(6,motor_driver.printState());
    printer.printValue(7,imu.printRollPitchHeading());        
    printer.printValue(8,imu.printAccels());
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-pcontrol.lastExecutionTime > LOOP_PERIOD ) {
    pcontrol.lastExecutionTime = currentTime;
    pcontrol.calculateControl(&state_estimator.state);
    motor_driver.driveForward(pcontrol.uL,pcontrol.uR);
  }

  if ( currentTime-adc.lastExecutionTime > LOOP_PERIOD ) {
    adc.lastExecutionTime = currentTime;
    adc.updateSample(); 
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read();     // blocking I2C calls
  }
  
  if (true){//(gps.loopTime(loopStartTime)) {
    gps.lastExecutionTime = currentTime;
    gps.read(&GPS); // blocking UART calls
  }

  if ( currentTime-state_estimator.lastExecutionTime > LOOP_PERIOD ) {
    state_estimator.lastExecutionTime = currentTime;
    state_estimator.updateState(&imu.state, &gps.state);
  }
  
  // uses the LED library to flash LED -- use this as a template for new libraries!
  if (currentTime-led.lastExecutionTime > LOOP_PERIOD) {
    led.lastExecutionTime = currentTime;
    //led.flashLED();
    int startT = millis();
    numBytes = BTSerial.available() + 1;
    if(numBytes>1){ 
      x = BTSerial.readBytesUntil(47,buff,10);
     BTSerial.clear();
    }
//      for (int i=0;i<numBytes;i++){
//        if(buff[i]==33){
//          //BTSerial.write(32);
//          sample();       
//          BTSerial.clear();
//          gpsSwitch = 1;
//          break;
//        }
//      }
//      printer.printMessage("Buff size is= " + String(x),10);
//      printer.printMessage("Buff is= " + String(buff),10);
//      printer.printMessage("Switch is =" +String(gpsSwitch),10);
        for(int i=0;i<4;i++){
          buff2[i] = buff[5+i];
           }
        float lat = *(float *)&buff; 
        float lon = *(float *)&buff2;
        printer.printMessage("Lat is = " + String(lat,5),10);
        printer.printMessage("Lon is = " + String(lon,5),10);
           //latSwitch = 0;
           
      int endT = millis() - startT;
      printer.printMessage("LED Loop Time is = " + String(endT),10);
      for(int i=0;i<4;i++){
          buff2[i]=0;
           }
      for(int i=0;i<9;i++){
         buff[i]=0;     
           }
      sendGPS.updateState();
           
  }

  if (currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }
}
void sample(){
  BTSerial.clear(); 
  for(int i=0;i<3;i++){
    BTSerial.write(51); 
  }
  //BTSerial.flush();
  delay(15);
}

