#include "SensorIMU.h"
#include "Printer.h"
extern Printer printer;

SensorIMU::SensorIMU(void)
  : DataSource("headingIMU,accelX,accelY,accelZ","float,float,float,float") {
}

void SensorIMU::init(void) {
  Serial.print("Initializing IMU... ");

  // Initialize the sensors.
  if(!gyro.begin()) {
    printer.printMessage("No gyro detected (IMU failed to initialize)",0);
  }

  if(!accelmag.begin(ACCEL_RANGE_4G)) {
    printer.printMessage("No FXOS8700 detected (IMU failed to initialize)",0);
  }
  // Filter expects 10 samples per second, adjust as necessary
  filter.begin(10);

  Serial.println("done!");
}

void SensorIMU::read(void) {
  sensors_event_t gyro_event;
  sensors_event_t accel_event;
  sensors_event_t mag_event;

  float x;
  float y;
  float z;

  float mx;
  float my;
  float mz;

  float gx;
  float gy;
  float gz;

  float ax;
  float ay;
  float az;
  // Try with no filter
  //simple()
  for(int i = 0; i < NUM_TO_AVERAGE; i++){
    // Get new data samples
    gyro.getEvent(&gyro_event);
    accelmag.getEvent(&accel_event, &mag_event);

      // Apply mag offset compensation (base values in uTesla)
    x = mag_event.magnetic.x - mag_offsets[0];
    y = mag_event.magnetic.y - mag_offsets[1];
    z = mag_event.magnetic.z - mag_offsets[2];

    // Apply mag soft iron error compensation
    mx = x * mag_ironcomp[0][0] + y * mag_ironcomp[0][1] + z * mag_ironcomp[0][2];
    my = x * mag_ironcomp[1][0] + y * mag_ironcomp[1][1] + z * mag_ironcomp[1][2];
    mz = x * mag_ironcomp[2][0] + y * mag_ironcomp[2][1] + z * mag_ironcomp[2][2];

    // Apply gyro zero-rate error compensation, convert to degrees/s from rad/s
    gx = (gyro_event.gyro.x + gyro_zero_offsets[0]) * 57.2958F;
    gy = (gyro_event.gyro.y + gyro_zero_offsets[1]) * 57.2958F;
    gz = (gyro_event.gyro.z + gyro_zero_offsets[2]) * 57.2958F;

    ax = accel_event.acceleration.x;
    ay = accel_event.acceleration.y;
    az = accel_event.acceleration.z;

    getOrientation(ax,ay,az,mx,my,mz);  // populate the this->simple field with simple orientation calcs
    state.roll = simple.roll;
    state.pitch = simple.pitch;
    headingArray[i] = simple.heading * 57.2958F;
    runningAverage += headingArray[i];
  }
  state.roll = simple.roll;
  state.pitch = simple.pitch;
  state.heading = runningAverage/NUM_TO_AVERAGE;
  runningAverage = 0; //reset the running average


  /////////// Old orientation estiamtion code from v1 ///////////////
  // // Update the filter
  // filter.update(gx, gy, gz,
  //               ax, ay, az,
  //               mx, my, mz);

  // // update the orientation data
  // state.roll = filter.getRoll();
  // state.pitch = filter.getPitch();
  // state.heading = filter.getYaw();

  // udpate the acceleration data
  acceleration.x = ax;
  acceleration.y = ay;
  acceleration.z = az;
}

String SensorIMU::printRollPitchHeading(void) {
  String printString = "IMU:";
  printString += " roll: ";
  printString += String(state.roll);
  printString += " pitch: ";
  printString += String(state.pitch);
  printString += " heading: ";
  printString += String(state.heading);
  return printString;
}

String SensorIMU::printSimple(void) {
  String printString = "IMU:";
  printString += " roll: ";
  printString += String(simple.roll);
  printString += " pitch: ";
  printString += String(simple.pitch);
  printString += " heading: ";
  printString += String(180.0F+simple.heading*180.0F/3.14159265F);
  return printString;
}

String SensorIMU::printAccels(void) {
  String printString = "IMU:";
  printString += " accel X: ";
  printString += String(acceleration.x);
  printString += " accel Y: ";
  printString += String(acceleration.y);
  printString += " accel Z: ";
  printString += String(acceleration.z);
  return printString;
}

size_t SensorIMU::writeDataBytes(unsigned char * buffer, size_t idx) {
  float * data_slot = (float *) &buffer[idx];
  // data_slot[0] = state.roll;
  // data_slot[1] = state.pitch;
  data_slot[0] = state.heading;
  data_slot[1] = acceleration.x;
  data_slot[2] = acceleration.y;
  data_slot[3] = acceleration.z;
  return idx + 4*sizeof(float);
}

void SensorIMU::getOrientation(float ax, float ay, float az, float mx, float my, float mz) {
  // copied from Adafruit_Simple_AHRS

  float const PI_F = 3.14159265F;

  // roll: Rotation around the X-axis. -180 <= roll <= 180
  // a positive roll angle is defined to be a clockwise rotation about the positive X-axis
  //
  //                    y
  //      roll = atan2(---)
  //                    z
  //
  // where:  y, z are returned value from accelerometer sensor
  simple.roll = (float)atan2(ay, az);

  // pitch: Rotation around the Y-axis. -180 <= roll <= 180
  // a positive pitch angle is defined to be a clockwise rotation about the positive Y-axis
  //
  //                                 -x
  //      pitch = atan(-------------------------------)
  //                    y * sin(roll) + z * cos(roll)
  //
  // where:  x, y, z are returned value from accelerometer sensor
  if (ay * sin(simple.roll) + az * cos(simple.roll) == 0)
    simple.pitch = ax > 0 ? (PI_F / 2) : (-PI_F / 2);
  else
    simple.pitch = (float)atan(-ax / (ay * sin(simple.roll) + az * cos(simple.roll)));

  // heading: Rotation around the Z-axis. -180 <= roll <= 180
  // a positive heading angle is defined to be a clockwise rotation about the positive Z-axis
  //
  //                                       z * sin(roll) - y * cos(roll)
  //   heading = atan2(--------------------------------------------------------------------------)
  //                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z * sin(pitch) * cos(roll))
  //
  // where:  x, y, z are returned value from magnetometer sensor
  simple.heading = (float)atan2(mz * sin(simple.roll) - my * cos(simple.roll), \
                               mx * cos(simple.pitch) + \
                               my * sin(simple.pitch) * sin(simple.roll) + \
                               mz * sin(simple.pitch) * cos(simple.roll));

}
