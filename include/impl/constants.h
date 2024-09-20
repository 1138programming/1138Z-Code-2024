#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.5;

const float KSplitTankLeftMult = 1.0;
const float KSplitTankRightMult = 1.0;

// drive base
const int KBackRightMotorPort = 13;
const int KMiddleRightMotorPort = 11;
const int KFrontRightMotorPort = 12;

const int KBackLeftMotorPort = 18; // rev
const int KMiddleLeftMotorPort = 20; // rev
const int KFrontLeftMotorPort = 19; // rev

// intake
const int KIntakeMotorPort = 8;


// odom
const int KInertialSensorPort = 5;
const float KOdomWheelSize = 4.125;
#endif