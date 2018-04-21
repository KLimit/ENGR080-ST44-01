#include "PControl.h"
#include "Printer.h"
#include <math.h>
#include "StateEstimator.h"
extern Printer printer;
extern SendGPS sendGPS;
extern StateEstimator stateEst;

inline float angleDiff(float a) {
  while (a<-PI) a += 2*PI;
  while (a> PI) a -= 2*PI;
  return a;
}

PControl::PControl(void) {
}

void PControl::init(const int totalWayPoints_in, const int stateDims_in, double * wayPoints_in, double followDist) {
  if (followDist > 0) {
    // we are saying that the pcontrol is a follower if it has a nonzero followdistance
    x_des = 0;
    y_des = 0;
    dist_des = followDist;
  } else {
    // when the PControl object is tied to the leader, initialize as normal
    totalWayPoints = totalWayPoints_in;
    stateDims = stateDims_in;
    wayPoints = wayPoints_in;
  }
}

void PControl::setAvgPower(float speedConstant) {
  // can be used to set the nominal speeds of individual PControl objects (robots)
  // default is still 5.0
  avgPower = speedConstant;
}

int PControl::getWayPoint(int dim) {
  // basically for getting the x and y coordinates of the current waypoint
  // not sure if it's better that x and y are in the same array now and we have
  // to multiply by the number of dimensions to move by coordinate set.
  // I guess it's more flexible this way.
  return wayPoints[currentWayPoint*stateDims+dim];
}

void PControl::updateFollowerWaypoint(SendGPS sendGPS) {
  // will primarily be used for updating the follower's waypoints
  // changes x_des and y_des
  // x_des and y_des are the properties of
  stateEst.latlonToXY((double) sendGPS.leaderLat, (double) sendGPS.leaderLon, x_des, y_des);
  // x_des = currentLeaderState->x;
  // y_des = currentLeaderState->y;
}

void PControl::calculateLeaderControl(state_t * state) {
  // USE ONLY WITH LEADER
  updatePoint(state->x, state->y);
  if (currentWayPoint == totalWayPoints) return; // stops motors at final point

  // set up variables (crappy practice but I took out the underscores to separate these from the object variables of the same name)
  float xdes = getWayPoint(0);
  float ydes = getWayPoint(1);

  yaw_des = calcDesiredHeading(ydes, state->y, xdes, state->x);
  yaw = state->heading;
  u = Kp * angleDiff(yaw_des - yaw);

  uL = max(0.0,min(255.0,(avgPower - u)*KlLead));
  uR = max(0.0,min(255.0,(avgPower + u)*KrLead));

}

void PControl::calculateFollowerControl(state_t * fState) {
  float fx = fState->x;
  float fy = fState->y;
  float fh = fState->heading;

  calculateNominal(fx, fy);
  calculateSteering(fx, fy, fh);
}

void PControl::calculateNominal(float x, float y) {
  // FOLLOWER
  // follower robot's state
  //distFromLeader = sqrt(pow(x-x_des, 2) + pow(y-y_des, 2));
  distFromLeader = sqrt(pow(x-0, 2) + pow(y+1, 2));

  float distError = dist_des - distFromLeader;
  if (distError >= 0) {
    // if the follower is closer than it should be, wait
    avgPower = 0;
  } else {
    avgPower = min(100,abs(Kpower*distError));
  }
}

void PControl::calculateSteering(float x, float y, float h) {
  // FOLLOWER
  // ANGLE TOWARD LEADER
  //yaw_des = calcDesiredHeading(y_des, y, x_des, x);
  yaw_des = calcDesiredHeading(-1, y, 0, x);
  yaw = h;
  u = Kp * angleDiff(yaw_des - yaw);
  uL = max(0.0, min(255.0, (avgPower - u) * KlFollow));
  uR = max(0.0, min(255.0, (avgPower + u) * KrFollow));

}

float PControl::calcDesiredHeading(float y_des, float y, float x_des, float x) {
  // Taken from the PControl that we implemented long ago for Lab 7
  return atan2(y_des - y, x_des - x);
}

String PControl::printLeaderString(void) {
  String printString = //"PControl: Yaw_Des: " + String(yaw_des*180.0/PI)
   // + " Yaw: " + String(yaw*180.0/PI)
     " u: " + String(u);
     //" uL " + String(uL);
    + " uR: " + String(uR);
   
    

  return printString;
}

String PControl::printFollowerString(void) {
  String printString = + " Dist from WP: " + String(distFromLeader) + " u: " + String(u)
    + " uL: " + String(uL)
    + " uR: " + String(uR);
     

  return printString;
}

String PControl::printWaypointUpdate(void) {
  String wayPointUpdate = "AvgP: " + String(avgPower)
    + " Dist from WP: " + String(distFromLeader);
    
  return wayPointUpdate;
}

void PControl::updatePoint(float x, float y) {
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
