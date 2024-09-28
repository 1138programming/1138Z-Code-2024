#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.5;

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


// odom
const int KInertialSensorPort = 5;
const float KOdomWheelSize = 4.125;

//brain
static vex::brain botBrain;

#endif