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
PID movementPID(0.0, 4.2, 0.0, 2.0, 100, -100, 0.1);
Movement botMovement(&robotBase, true, true);

Controller mainController(vex::controllerType::primary);
vex::motor intakeMotor(KIntakeMotorPort, false); // rev so it starts the correct dir
vex::motor intakeHoodMotor(KIntakeHoodMotorPort, true);
Hang botHangPneumatics;

AutonSelector autonSelector(mainController.getVexObject());

vex::inertial* internalGyro = new vex::inertial(KInertialSensorPort);
Gyro* botGyro = new Gyro(internalGyro);
Odometry* botOdom = new Odometry(KOdomWheelSize, &robotBase, botGyro);
OdomMovement* gamer = new OdomMovement(botOdom, &botMovement, botGyro, &movementPID, &turningPID, &autonSelector);

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
  robotBase.resetAllEncoders();
  botGyro->resetGyroWithWait();

  autonSelector.add_auton("Do nothing");
  autonSelector.add_auton("Drive Forwards");
  autonSelector.add_auton("2 Stack AWP +0");
  autonSelector.add_auton("2 Stack +2");
  autonSelector.add_auton("2 Mogo +2");
  autonSelector.add_auton("testing");

  // autonSelector.updateScreen();
  
  while(!Competition.isEnabled()) {
    autonSelector.update();
    vex::wait(5, vex::msec);
  }

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
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
  robotBase.resetAllEncoders();
  botGyro->resetGyroWithWait();
  uint32_t setTime;
  // int currentAuton = autonSelector.getCurrentAuton();
  // bool redAuton = autonSelector.getAutonRedSide();
  // botGyro->resetGyroWithWait();

  //temporary for testing specific auton
  autonSelector.setAuton(5);
  autonSelector.setAutonRedSide(true);

  // 0 = nothing
  // 1 = drive forwards
  // 2 = two stack side AWP +0
  // 3 = two stack side 2 scored and touching
  // 4 = two mogo side 2 scored and touching
  switch(autonSelector.getCurrentAuton()) {
    //do nothing
    case 0: {
      break;
    }

    //drive forwards
    case 1: {
      gamer->fixed(-15);
    }
    
    // two stack side AWP +0
    case 2: {
      mogoMech.set(false);
      gamer->fixed(-33.0);
      mogoMech.set(true);
      gamer->fixed(-7.0);
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(315.0, 6.0, autonSelector.getAutonRedSide());
      mogoMech.set(false);
      gamer->fixed(16.971);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-44.0);
      gamer->turnToPosPIDSideFixed(45.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-16.971);
      mogoMech.set(true);
      intakeMotor.spin(vex::forward, 100, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-48.0);
      break;
    }

    //two stack side 2 scored and touching
    case 3: {
      mogoMech.set(false);
      gamer->fixed(-28.0);
      gamer->fixedSlow(-4.0, 50);
      mogoMech.set(true);
      gamer->fixed(-8.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 750;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(42.0);
      break;
    }
    //two mogo side 2 scored and touching
    case 4: {
      mogoMech.set(false);
      gamer->fixed(-28.0);
      gamer->fixedSlow(-4.0, 50);
      mogoMech.set(true);
      gamer->fixed(-8.0);
      intakeMotor.spin(vex::forward, 80, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 80, vex::pct);
      intakeHoodMotor.spin(vex::forward, 100, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      intakeHoodMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(34.0);
      gamer->fixedSlow(8.0, 50);
      break;
    }
    case 5: {
      gamer->fixed(24.0);
      break;
    }
  }
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
  unsigned long frame = 0;
  mogoMechToggle.setEnabled();
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