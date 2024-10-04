#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.75;

const float KSplitTankLeftMult = 1.0;
const float KSplitTankRightMult = 1.0;

// drive base
const int KBackRightMotorPort = vex::PORT13; // rev
const int KMiddleRightMotorPort = vex::PORT12;
const int KFrontRightMotorPort = vex::PORT11; // rev

const int KBackLeftMotorPort = vex::PORT18;
const int KMiddleLeftMotorPort = vex::PORT20; // rev
const int KFrontLeftMotorPort = vex::PORT19;


const bool KBackRightMotorRev = true;
const bool KMiddleRightMotorRev = false;
const bool KFrontRightMotorRev = true;

const bool KBackLeftMotorRev = false;
const bool KMiddleLeftMotorRev = true;
const bool KFrontLeftMotorRev = false;

// intake
const int KIntakeMotorPort = vex::PORT1;
const int KIntakeHoodMotorPort = vex::PORT2;
const float KIntakeMotorSpeedMult = 1.0;


// odom
const int KInertialSensorPort = vex::PORT16;
const float KOdomWheelSize = 2.75;
const double kInchesFixMult = 0.018420820508516; // TODO: FIX THISS!!!!!!
//const float kInchesFixMult = 0.0221;

//brain
static vex::brain botBrain;

#endif