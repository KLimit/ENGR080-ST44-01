#ifndef __PCONTROL_H__
#define __PCONTROL_H__

#define SUCCESS_RADIUS 2.0 // success radius in meters

#include <Arduino.h>
#include "MotorDriver.h"
#include "StateEstimator.h"
#include "SendGPS.h"
extern MotorDriver motorDriver;

class PControl {
public:
  PControl(void);

  // defines the waypoints used for pControl
  void init(const int totalWayPoints_in, const int stateDims_in, double * wayPoints_in, double followDist);

  void setAvgPower(float speedConstant);

  // sets the motor speeds using P-Control
  void calculateLeaderControl(state_t * state);

  void calculateFollowerControl(state_t * state);
  void calculateNominal(float x, float y);
  void calculateSteering(float x, float y, float h);


  void updatePoint(float x, float y);
  void updateFollowerWaypoint(SendGPS sendGPS);

  String printLeaderString(void);
  String printFollowerString(void);
  String printWaypointUpdate(void);

  int lastExecutionTime = -1;

  // control fields
  float yaw_des;
  float yaw;
  float dist;            // distance to 
  float distFromLeader;   //distance from leader
  float u;               // control effort
  float Kp=10.0;         // proportional control gain
  float KrLead = 0.7;          // right motor gain correction for leader
  float KlLead=2;          // left motor gain correction for leader
  float KrFollow = 1.0;
  float KlFollow = 1;
  float Kpower = 0.05;   // average power proportional control gain (FOLLOWER)
  float Ksteering = 10.0;// steering proportional control gain (FOLLOWER)
  float avgPower = 15.0;  // average forward thrust
  double uR;            // right motor effort
  double uL;             // left motor effort



private:

  // updates the current follower waypoint if necessary

  int getWayPoint(int dim);

  float calcDesiredHeading(float y_des, float y, float x_des, float x);

  int totalWayPoints, stateDims;
  double * wayPoints;
  double * followerPoints;
  int currentWayPoint = 0;

  // THESE ARE TO BE USED EXCLUSIVELY FOR THE FOLLOWER SO THAT IT HAS A SINGLE
  // SET OF WAYPOINTS THAT ARE CONTINUOUSLY UPDATED
  // "Follower WayPoint X/Y"
  double x_des;
  double y_des;
  double dist_des;
};

#endif
