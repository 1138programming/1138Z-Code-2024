#ifndef CONSTANTS_H
#define CONSTANTS_H

// driving
const float KSplitArcadeForwardMult = 1.0;
const float KSplitArcadeTurningMult = 0.5;

const float KSplitTankLeftMult = 1.0;
const float KSplitTankRightMult = 1.0;

// drive base
const int KBackRightMotorPort = PORT13;
const int KMiddleRightMotorPort = PORT11;
const int KFrontRightMotorPort = PORT12;

const int KBackLeftMotorPort = PORT18; // rev
const int KMiddleLeftMotorPort = PORT20; // rev
const int KFrontLeftMotorPort = PORT19; // rev

// intake
const int KIntakeMotorPort = PORT1;


// odom
const int KInertialSensorPort = 5;
const float KOdomWheelSize = 4.125;
#endif