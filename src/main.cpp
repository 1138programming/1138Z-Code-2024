/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       josh-duross                                               */
/*    Created:      12/28/2023, 1:38:47 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "impl/constants.h"
#include "lib/bot/gyro.hpp"
#include "lib/commands/movement.hpp"
#include "lib/resources/controller.hpp"
#include "lib/resources/PID.hpp"
#include "lib/resources/holdable.hpp"
#include "impl/bot/intake.hpp"
#include "impl/bot/hang.hpp"
#include "lib/commands/odomMovement.hpp"


using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
std::vector<vex::motor*> leftMotors{new vex::motor(KBackLeftMotorPort, KBackLeftMotorRev), new vex::motor(KMiddleLeftMotorPort, KMiddleLeftMotorRev), new vex::motor(KFrontLeftMotorPort, KFrontLeftMotorRev)};
std::vector<vex::motor*> rightMotors{new vex::motor(KBackRightMotorPort, KBackRightMotorRev), new vex::motor(KMiddleRightMotorPort, KMiddleRightMotorRev), new vex::motor(KFrontRightMotorPort, KFrontRightMotorRev)};
Base robotBase(leftMotors, rightMotors);
PID turningPID(0.0, -0.25, 0.0, 0.0, 100, -100, 0.4);
PID movementPID(0.0, 275, 0.0, 0.0, 100, -100, 0.1);
Movement botMovement(&robotBase, true, true);

Controller mainController(vex::controllerType::primary);
vex::motor intakeMotor(KIntakeMotorPort, true); // rev so it starts the correct dir
vex::motor intakeHoodMotor(KIntakeHoodMotorPort, true);
Hang botHangPneumatics;

vex::inertial* internalGyro = new vex::inertial(KInertialSensorPort);
Gyro* botGyro = new Gyro(internalGyro);
Odometry* botOdom = new Odometry(KOdomWheelSize, &robotBase, botGyro);
OdomMovement* gamer = new OdomMovement(botOdom, &botMovement, botGyro, &movementPID, &turningPID);

Toggleable intakeEnabled;
Toggleable intakeReversed;

Toggleable mogoMechToggle;
vex::digital_out mogoMech(botBrain.ThreeWirePort.A);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  uint32_t setTime;

  robotBase.resetAllEncoders();
  botGyro->resetGyro();

  //this code sucks kys - bronson

  // move forward + intake
  intakeMotor.spin(vex::forward, -100, vex::pct);
  gamer->fixed(18.0);

  // spin intake for 500ms
  setTime = vex::timer::system() + 500;
  while (vex::timer::system() <= setTime) {
    vex::wait(5, vex::msec);
  }
  intakeMotor.spin(vex::forward, 0, vex::pct);

  while(!botGyro->isResetFinished()) {
    // do nothing
    vex::wait(5, vex::msec);
  }

  //move back, spin, then move more forward (don't get hit on thing)
  gamer->fixed(-10.0);
      // force turn left
      // setTime = vex::timer::system() + 60;
      // while (vex::timer::system() < setTime) {
      //   botMovement.turn(100);
      // }
  gamer->turnToPosPID(180.0, 8.0);
  gamer->fixed(18.0);

  // turn and go forward into goal (multiple steps)
    gamer->turnToPosPID(140.0, 8.0);
    gamer->fixed(26.0);
    gamer->turnToPosPID(90, 8.0);
    // spin outtake before we go into goal... (and go in)
      intakeMotor.spin(vex::forward, 100, vex::pct);
      setTime = vex::timer::system() + 200;
      while (vex::timer::system() < setTime) {
        vex::wait(5, vex::msec);
      }
      gamer->fixed(12.0);
    // leave goal and turn to triballs
    gamer->fixed(-22.0);
    intakeMotor.spin(vex::forward, -100, vex::pct);
    gamer->turnToPosPID(33.0, 8.0);
    gamer->fixed(46.0);
    intakeMotor.spin(vex::forward, 0, vex::pct);
    // turn back to goal and outtake
    gamer->turnToPosPID(160.0, 8.0);
    intakeMotor.spin(vex::forward, 100, vex::pct);
    gamer->fixed(30.0);
    // turn to goal
    // sex
      gamer->fixed(30.0);
      intakeMotor.spin(vex::forward, 0, vex::pct);
      while(true) {
        gamer->fixed(-10.0);
        vex::wait(50, vex::msec);
        gamer->fixed(10.0);
        vex::wait(50, vex::msec);
      }


  // gamer->turnToPosPID(90.0, 0.1);

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
      // This is the main execution loop for the user control program.
      // Each time through the loop your program should update motor + servo
      // values based on feedback from the joysticks.

      // ........................................................................
      // Insert user code here. This is where you use the joystick values to
      // update your motors, etc.
      // ........................................................................
      
      botMovement.driveSplitArcade(&mainController);
      //botHangPneumatics.update(mainController.getButton(BUTTON_B));

      if (intakeEnabled.isEnabled()) {
        intakeMotor.spin(vex::forward, (intakeReversed.isEnabled() ? -100 : 100), vex::pct); // spin both correct dir
        intakeHoodMotor.spin(vex::forward, (intakeReversed.isEnabled() ? -100 : 100), vex::pct);
      }
      else {
        intakeMotor.spin(vex::forward, 0, vex::pct);
        intakeHoodMotor.spin(vex::forward, 0, vex::pct);
      }

      mogoMech.set(mogoMechToggle.isEnabled());

      intakeEnabled.update(mainController.getButton(BUTTON_R1));
      intakeReversed.update(mainController.getButton(BUTTON_R2));

      if(mogoMechToggle.isEnabled()) {
        mogoMechToggle.update(mainController.getButton(BUTTON_L2));
      }
      else {
        mogoMechToggle.update(mainController.getButton(BUTTON_L1));
      }
        
      wait(5, msec); // Sleep the task for a short amount of time to prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // free mem
    for(int i = 0; i < leftMotors.size(); i++) {
      delete leftMotors.at(i);
    }
    for (int i = 0; i < rightMotors.size(); i++) {
      delete rightMotors.at(i);
    }
    delete botGyro;
    delete botOdom;
    delete gamer;

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}