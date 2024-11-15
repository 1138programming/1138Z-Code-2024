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
#include "impl/bot/intake.hpp"// file not used
#include "impl/bot/hang.hpp"
#include "lib/commands/odomMovement.hpp"
#include "impl/bot/autonSelector.hpp"


using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
std::vector<vex::motor*> leftMotors{new vex::motor(KBackLeftMotorPort, vex::ratio6_1, KBackLeftMotorRev), new vex::motor(KMiddleLeftMotorPort, vex::ratio6_1, KMiddleLeftMotorRev), new vex::motor(KFrontLeftMotorPort, vex::ratio6_1, KFrontLeftMotorRev)};
std::vector<vex::motor*> rightMotors{new vex::motor(KBackRightMotorPort, vex::ratio6_1, KBackRightMotorRev), new vex::motor(KMiddleRightMotorPort, vex::ratio6_1, KMiddleRightMotorRev), new vex::motor(KFrontRightMotorPort, vex::ratio6_1, KFrontRightMotorRev)};
Base robotBase(leftMotors, rightMotors);
PID turningPID(0.0, -0.4, 0.0, 0.0, 100, -100, 0.4);
PID movementPID(0.0, 200.0, 0.0, 0.0, 100, -100, 0.1);
Movement botMovement(&robotBase, true, true);

Controller mainController(vex::controllerType::primary);
vex::motor intakeMotor(KIntakeMotorPort, false); // rev so it starts the correct dir
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

AutonSelector autonSelector(mainController.getVexObject());

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
  autonSelector.add_auton("Do nothing");
  autonSelector.add_auton("2 Stack AWP +0");
  autonSelector.add_auton("Right + Mid");
  autonSelector.add_auton("Right (No Mid)");

  autonSelector.updateScreen();
  
  while (!Competition.isAutonomous() || !Competition.isDriverControl()) {
    autonSelector.update();
    vex::wait(5, vex::msec);
  }

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  robotBase.resetAllEncoders();
  botGyro->resetGyroWithWait();
  //intakeMotor.setStopping(vex::hold);
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
  int currentAuton = autonSelector.getCurrentAuton();
  bool redAuton = autonSelector.getAutonRedSide();

  switch(currentAuton) {
    //do nothing
    case 0: {
      //cooking cooking cooking
    }
    // two mogo side
    case 1: {
      mogoMech.set(true);
      gamer->fixed(-38.75);
      mogoMech.set(false);
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPID(90.0, 6.0);
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPID(315.0, 6.0);
      mogoMech.set(false);
      gamer->fixed(16.971);
      gamer->turnToPosPID(90.0, 6.0);
      gamer->fixed(48.0);
      gamer->turnToPosPID(45.0, 6.0);
      gamer->fixed(16.971);
      mogoMech.set(true);
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->turnToPosPID(270.0, 6.0);
      gamer->fixed(24);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);
      gamer->turnToPosPID(90.0, 6.0);
      gamer->fixed(48.0);
      break;
    }

  }

  // go to mogo, pick up, & score preload
  mogoMech.set(true);
  gamer->fixed(-30.0);
  mogoMech.set(false);
  intakeMotor.spin(vex::forward, 100, vex::pct);
  intakeHoodMotor.spin(vex::forward, 100, vex::pct);
  setTime = vex::timer::system() + 2500;
  while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
  intakeMotor.spin(vex::forward, 0, vex::pct);
  intakeHoodMotor.spin(vex::forward, 0, vex::pct);

  // let go of mogo & intake next disc
  mogoMech.set(true);
  gamer->turnToPosPID(59.744, 6.0);
  intakeMotor.spin(vex::forward, 100, vex::pct);
  gamer->fixed(27.785);
  setTime = vex::timer::system() + 100;
  while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for full intake hold
  intakeMotor.spin(vex::forward, 0, vex::pct);

  // move to first move of mogo
  gamer->turnToPosPID(270.0, 6.0);
  gamer->fixed(-13.856);
  gamer->turnToPosPID(0.0, 6.0);
  gamer->fixed(-24.0);
  // gamer->turnToPosPID(330.0, 6.0);
  // gamer->fixed(-27.71);

  // move to mogo
  mogoMech.set(true);
  gamer->turnToPosPID(30.0, 6.0);
  gamer->fixed(-18.46);
  mogoMech.set(false);

  // move back & outtake
  gamer->fixed(10.0);
  intakeMotor.spin(vex::forward, 100, vex::pct);
  intakeHoodMotor.spin(vex::forward, 100, vex::pct);
  // setTime = vex::timer::system() + 2000;
  // while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for full score
  // intakeMotor.spin(vex::forward, 0, vex::pct);
  // intakeHoodMotor.spin(vex::forward, 0, vex::pct);
  // gamer->turnToPosPID(90.0, 6.0);
  // gamer->fixed(-29.0);
  // gamer->turnToPosPID(0.0, 6.0);
  // gamer->fixed(-8.67025403);




  //this code sucks kys - bronson

  // move forward + intake
  //intakeMotor.spin(vex::forward, -100, vex::pct);
  //gamer->turnToPosPID(180.0, 4.0);

  // pick up MOGO & turn to ring
  // mogoMech.set(true);
  // gamer->fixed(-28.806);
  // mogoMech.set(false);
  // intakeMotor.spin(vex::forward, 100, vex::pct);
  // gamer->turnToPosPID(58.33, 6.0); // thank the lord for trig

  // // (hopefully) intake ring onto mogo
  // gamer->fixed(20.1996);
  // intakeMotor.spin(vex::forward, 0, vex::pct);
  // gamer->fixed(-6.0);
  // intakeMotor.spin(vex::forward, 100, vex::pct);
  // intakeHoodMotor.spin(vex::forward, 100, vex::pct);
  // gamer->fixed(8.0);

  // setTime = vex::timer::system() + 500; // wait so bot hopefully scores
  // while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);} // wait loop

  // // let go of mogo 
  // mogoMech.set(true);
  // gamer->fixed(-8.0); //move it out of the way
  // gamer->fixed(8.0);
  // // if we haven't scored it's a lost cause
  // intakeMotor.spin(vex::forward, 0, vex::pct);
  // intakeHoodMotor.spin(vex::forward, 0, vex::pct);

  // // turn & move to second mogo
  // gamer->turnToPosPID(3.88, 6.0);
  // gamer->fixed(-34.215); // makes sense
  // // gamer->turnToPosPID(5.0, 6.0); // we are dangerously close to DQ- tries to help w/ that
  // // gamer->fixed(-5.0);

  // // clamp mogo & move AWAY from line
  // mogoMech.set(false);
  // gamer->fixed(20.0);


  // //gamer->turnToPosPID(240.0, 8.0);

  // // // spin intake for 500ms
  // // setTime = vex::timer::system() + 500;
  // // while (vex::timer::system() <= setTime) {
  // //   vex::wait(5, vex::msec);
  // // }
  // // intakeMotor.spin(vex::forward, 0, vex::pct);

  // // while(!botGyro->isResetFinished()) {
  // //   // do nothing
  // //   vex::wait(5, vex::msec);
  // // }

  // // //move back, spin, then move more forward (don't get hit on thing)
  // // gamer->fixed(-10.0);
  // //     // force turn left
  // //     // setTime = vex::timer::system() + 60;
  // //     // while (vex::timer::system() < setTime) {
  // //     //   botMovement.turn(100);
  // //     // }
  // // gamer->turnToPosPID(180.0, 8.0);
  // // gamer->fixed(18.0);

  // // // turn and go forward into goal (multiple steps)
  // //   gamer->turnToPosPID(140.0, 8.0);
  // //   gamer->fixed(26.0);
  // //   gamer->turnToPosPID(90, 8.0);
  // //   // spin outtake before we go into goal... (and go in)
  // //     intakeMotor.spin(vex::forward, 100, vex::pct);
  // //     setTime = vex::timer::system() + 200;
  // //     while (vex::timer::system() < setTime) {
  // //       vex::wait(5, vex::msec);
  // //     }
  // //     gamer->fixed(12.0);
  // //   // leave goal and turn to triballs
  // //   gamer->fixed(-22.0);
  // //   intakeMotor.spin(vex::forward, -100, vex::pct);
  // //   gamer->turnToPosPID(33.0, 8.0);
  // //   gamer->fixed(46.0);
  // //   intakeMotor.spin(vex::forward, 0, vex::pct);
  // //   // turn back to goal and outtake
  // //   gamer->turnToPosPID(160.0, 8.0);
  // //   intakeMotor.spin(vex::forward, 100, vex::pct);
  // //   gamer->fixed(30.0);
  // //   // turn to goal
  // //   // sex
  // //     gamer->fixed(30.0);
  // //     intakeMotor.spin(vex::forward, 0, vex::pct);
  // //     while(true) {
  // //       gamer->fixed(-10.0);
  // //       vex::wait(50, vex::msec);
  // //       gamer->fixed(10.0);
  // //       vex::wait(50, vex::msec);
  // //     }


  // // // gamer->turnToPosPID(90.0, 0.1);

  // // // ..........................................................................
  // // // Insert autonomous user code here.
  // // // ..........................................................................
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
  robotBase.resetAllEncoders();
  botGyro->resetGyroWithWait();
  unsigned long frame = 0;
  // User control code here, inside the loop
  while (1) {
      // This is the main execution loop for the user control program.
      // Each time through the loop your program should update motor + servo
      // values based on feedback from the joysticks.

      // ........................................................................
      // Insert user code here. This is where you use the joystick values to
      // update your motors, etc.
      // ........................................................................
      
      botOdom->pollAndUpdateOdom();
      if (frame % 20 == 0) {
        mainController.getVexObject()->Screen.clearLine(0);
        mainController.getVexObject()->Screen.setCursor(0, 0);
        mainController.getVexObject()->Screen.print("%f, %f", botOdom->getX(), botOdom->getY());
      }

      botMovement.driveSplitArcade(&mainController);
      //botHangPneumatics.update(mainController.getButton(BUTTON_B));

      if (intakeEnabled.isEnabled()) {
        intakeMotor.spin(vex::forward, (intakeReversed.isEnabled() ? -(KIntakeMotorSpeedMult * 100) : (KIntakeMotorSpeedMult * 100)), vex::pct); // spin both correct dir
        intakeHoodMotor.spin(vex::forward, (intakeReversed.isEnabled() ? -(KIntakeHoodMotorSpeedMult * 100) : (KIntakeHoodMotorSpeedMult * 100)), vex::pct);
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
        
      //prints pos of robot every frame on the brain

      frame++;
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
    // for(int i = 0; i < leftMotors.size(); i++) {
    //   delete leftMotors.at(i);
    // }
    // for (int i = 0; i < rightMotors.size(); i++) {
    //   delete rightMotors.at(i);
    // }
    //delete botGyro;
    // delete botOdom;
    // delete gamer;

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}