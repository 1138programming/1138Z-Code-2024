#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.75;

const float KSplitTankLeftMult = 1.0;
const float KSplitTankRightMult = 1.0;

// drive base
const int KBackRightMotorPort = vex::PORT10; // rev
const int KMiddleRightMotorPort = vex::PORT9; // rev
const int KFrontRightMotorPort = vex::PORT8; //rev

const int KBackLeftMotorPort = vex::PORT17;
const int KMiddleLeftMotorPort = vex::PORT19;
const int KFrontLeftMotorPort = vex::PORT18;


const bool KBackRightMotorRev = true;
const bool KMiddleRightMotorRev = true;
const bool KFrontRightMotorRev = true;

const bool KBackLeftMotorRev = false;
const bool KMiddleLeftMotorRev = false;
const bool KFrontLeftMotorRev = false;

// intake
const int KIntakeMotorPort = vex::PORT1;
const float KIntakeMotorSpeedMult = 1.0;

//LadyBrown
const int KWallStakeMotorPort = vex::PORT12;
const int KRotationSensorPort = -1; //TBD

// odom
const int KInertialSensorPort = vex::PORT5;
const float KOdomWheelSize = 2.7;
const double kInchesFixMult = 0.018420820508516; // TODO: FIX THISS!!!!!!
//const float kInchesFixMult = 0.0221;

//brain
static vex::brain botBrain;

#endif