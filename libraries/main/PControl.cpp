#include "PControl.h"
#include "Printer.h"
extern Printer printer;

inline float angleDiff(float a) {
  while (a<-PI) a += 2*PI;
  while (a> PI) a -= 2*PI;
  return a;
}

PControl::PControl(void) {
}

void PControl::init(const int totalWayPoints_in, const int stateDims_in, double * wayPoints_in, bool isLeader) {
  if (isLeader) {
    // when the PControl object is tied to the leader, initialize as normal
    totalWayPoints = totalWayPoints_in;
    stateDims = stateDims_in;
    wayPoints = wayPoints_in;
  } else {
    // when initialized for the follower, don't worry about totalWayPoints or
    // anything like that; instead just use the updatable doubles
    fwpX = 0;
    fwpY = 0;
  }
}

int PControl::getWayPoint(int dim) {
  // basically for getting the x and y coordinates of the current waypoint
  // not sure if it's better that x and y are in the same array now and we have
  // to multiply by the number of dimensions to move by coordinate set.
  // I guess it's more flexible this way.
  return wayPoints[currentWayPoint*stateDims+dim];
}

void PControl::updateFollowerWaypoint(state_t * gpsCoordinates) {
  // will primarily be used for updating the follower's waypoints
  // changes fwpX and fwpY

int x_des = gpsCoordinates->x;
int y_des = gpsCoordinates->y;

}

void PControl::calculateControl(state_t * state) {
  updatePoint(state->x, state->y);
  if (currentWayPoint == totalWayPoints) return; // stops motors at final point

  // set up variables
  int x_des = getWayPoint(0);
  int y_des = getWayPoint(1);

  yaw_des = atan2(y_des - state->y, x_des - state->x);
  yaw = state->heading;
  u = Kp*angleDiff(yaw_des - yaw);

  uL = max(0.0,min(255.0,(avgPower - u)*Kl));
  uR = max(0.0,min(255.0,(avgPower + u)*Kr));

}

void PControl::calculateNominal(state_t * fState) {
  int x = fState->x;
  int y = fState->y;

  int dist = sqrt((x_dex-x))
  
}

void PControl::calculateSteering(state_t * fState) {
  // for controlling the follower's Steering
  // ANGLE TOWARD LEADER
}

String PControl::printString(void) {
  String printString = "PControl: Yaw_Des: " + String(yaw_des*180.0/PI)
    + " Yaw: " + String(yaw*180.0/PI)
    + " u: " + String(u);

  return printString;
}

String PControl::printWaypointUpdate(void) {
  String wayPointUpdate = "PControl: Current Waypoint: " + String(currentWayPoint)
    + " Distance from Waypoint: " + String(dist);
  return wayPointUpdate;
}

void PControl::updatePoint(float x, float y, bool isLeader) {
  if (currentWayPoint == totalWayPoints) return; // don't check if finished

  int x_des = getWayPoint(0);
  int y_des = getWayPoint(1);

  // distance from current position to current waypoint
  dist = sqrt(pow(x-x_des,2) + pow(y-y_des,2));

  if (dist < SUCCESS_RADIUS && currentWayPoint < totalWayPoints) {
    String changingWPMessage = "Got to waypoint " + String(currentWayPoint)
      + ", now directing to next point";
    int cwpmTime = 20;
    currentWayPoint++;
    if (currentWayPoint == totalWayPoints) {
      changingWPMessage = "Congratulations! You completed the path! Stopping motors.";
      uR=0;
      uL=0;
      cwpmTime = 0;
    }
    printer.printMessage(changingWPMessage,cwpmTime);
  }
}
