#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.75;

const float KSplitTankLeftMult = 1.0;
const float KSplitTankRightMult = 1.0;

// drive base
const int KBackRightMotorPort = vex::PORT20; // rev
const int KMiddleRightMotorPort = vex::PORT7; // rev
const int KFrontRightMotorPort = vex::PORT8; 

const int KBackLeftMotorPort = vex::PORT19; // rev
const int KMiddleLeftMotorPort = vex::PORT18; // rev
const int KFrontLeftMotorPort = vex::PORT9;


const bool KBackRightMotorRev = true;
const bool KMiddleRightMotorRev = true;
const bool KFrontRightMotorRev = false;

const bool KBackLeftMotorRev = false;
const bool KMiddleLeftMotorRev = false;
const bool KFrontLeftMotorRev = true;

// intake
const int KIntakeMotorPort = vex::PORT10;
const int KIntakeHoodMotorPort = vex::PORT1;
const float KIntakeMotorSpeedMult = 0.5;
const float KIntakeHoodMotorSpeedMult = 1.0;


// odom
const int KInertialSensorPort = vex::PORT16;
const float KOdomWheelSize = 2.75;
const double kInchesFixMult = 0.018420820508516; // TODO: FIX THISS!!!!!!
//const float kInchesFixMult = 0.0221;

//brain
static vex::brain botBrain;

#endif