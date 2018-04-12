#ifndef __PCONTROL_H__
#define __PCONTROL_H__

#define SUCCESS_RADIUS 2.0 // success radius in meters

#include <Arduino.h>
#include "MotorDriver.h"
#include "StateEstimator.h"
extern MotorDriver motorDriver;

class PControl {
public:
  PControl(void);

  // defines the waypoints used for pControl
  void init(const int totalWayPoints_in, const int stateDims_in, double * wayPoints_in);

  // updates the set of waypoint values that the follower uses
  void updateFollowerWaypoint();

  // sets the motor speeds using P-Control
  void calculateControl(state_t * state);

  void calculateNominal(state_t* fState);
  void caclculateSteering(state_t* fState);

  String printString(void);
  String printWaypointUpdate(void);

  int lastExecutionTime = -1;

  // control fields
  float yaw_des;
  float yaw;
  float dist;            // distance to waypoint
  float u;               // control effort
  float Kp=10.0;         // proportional control gain
  float Kr=1.0;          // right motor gain correction
  float Kl=1.0;          // left motor gain correction
  float avgPower = 5.0;  // average forward thrust
  double uR;             // right motor effort
  double uL;             // left motor effort


private:

  // updates the current waypoint if necessary
  void updatePoint(float x, float y);

  int getWayPoint(int dim);

  int totalWayPoints, stateDims;
  double * wayPoints;
  int currentWayPoint = 0;

  // THESE ARE TO BE USED EXCLUSIVELY FOR THE FOLLOWER SO THAT IT HAS A SINGLE
  // SET OF WAYPOINTS THAT ARE CONTINUOUSLY UPDATED
  // "Follower WayPoint X/Y"
  double fwpX;
  double fwpY;

};

#endif
