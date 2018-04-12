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
//#include <SendGPS.h>
#define mySerial Serial1
#include <LED.h>  // A template of a data soruce library
#define BTSerial Serial3

// Phake Lake Origin
//#define ORIGIN_LAT  34.109463
//#define ORIGIN_LON  117.712776

// Axelrood Pool Origin
#define ORIGIN_LAT  34.10055556
#define ORIGIN_LON  117.70611111
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


// loop start recorder
int loopStartTime;
int currentTime;
int current_way_point = 0;
char x = 0;
int Listen = 0;
int kek = 0;
int loopCounter = 0;
float lat;
byte latBytes[4];
byte lonBytes[4];
float lon;
////////////////////////* Setup *////////////////////////////////

void setup() {
  BTSerial.begin(38400);
  pinMode(9,OUTPUT);
  pinMode(2,INPUT_PULLUP);
  digitalWrite(9,LOW);
  logger.include(&imu);
  logger.include(&gps);
  logger.include(&state_estimator);
  logger.include(&motor_driver);
//  logger.include(&sendGPS);
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
     int sent;
    int ledStart = millis();
    float2Bytes(gps.state.lat,&latBytes[0]);
    float2Bytes(gps.state.lon,&lonBytes[0]);
       for(int i=0;i<1;i++){
       sent = BTSerial.write(latBytes,4);
       BTSerial.write(63);
       BTSerial.write(lonBytes,4);
       BTSerial.write(47);
       }
       BTSerial.flush();
     int ledEnd = millis() - ledStart;
     float lat = *(float *)&latBytes; 
     float lon = *(float *)&lonBytes;
     printer.printMessage("Sent bytes: " + String(sent),10);
     printer.printMessage("Lat is = " + String(lat,5),10);
     printer.printMessage("Lon is = " + String(lon,5),10);
     printer.printMessage("LED loop time is: " + String(ledEnd),10);
  }

  if (currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }
  if ((loopCounter)%1000000==0){
    sendPulse();
  }
loopCounter = loopCounter + 1;
}
void float2Bytes(float val,byte* bytes_array){
  // Create union of shared memory space
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  // Overite bytes of union with float variable
  u.float_variable = val;
  // Assign bytes to input array
  memcpy(bytes_array, u.temp_array, 4);
}
void sendPulse(){
// for(int i=0;i<2;i++){
//       BTSerial.write(33);
//       }
//       BTSerial.flush();
//       Listen = 1;
//    while(Listen==1){
//      if(BTSerial.available()>0){
//      x = BTSerial.read();
//      BTSerial.clear();
//      break;
//      } 
//      else if(digitalRead(2)==LOW){
//        break;
//      }
//    }
    if (x == 51){
        digitalWrite(9,HIGH);
        delay(10);
        digitalWrite(9,LOW);
        //Listen = 0;
        //x = 0;
      }
}

