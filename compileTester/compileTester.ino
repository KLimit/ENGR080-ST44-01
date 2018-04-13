#include<PControl.h>
#include<StateEstimator.h>

StateEstimator stateEst;
const float origin_lat = 34.106465;
const float origin_lon = -117.712488;

void setup() {
  // put your setup code here, to run once:
  stateEst.init(origin_lat, origin_lon);

}

void loop() {
  // put your main code here, to run repeatedly:

}
