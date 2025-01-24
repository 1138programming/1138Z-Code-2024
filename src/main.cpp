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
#include "impl/bot/ladyBrown.hpp"
#include "lib/commands/odomMovement.hpp"
#include "impl/bot/autonSelector.hpp"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
std::vector<vex::motor*> leftMotors{new vex::motor(KBackLeftMotorPort, vex::ratio6_1, KBackLeftMotorRev), new vex::motor(KMiddleLeftMotorPort, vex::ratio6_1, KMiddleLeftMotorRev), new vex::motor(KFrontLeftMotorPort, vex::ratio6_1, KFrontLeftMotorRev)};
std::vector<vex::motor*> rightMotors{new vex::motor(KBackRightMotorPort, vex::ratio6_1, KBackRightMotorRev), new vex::motor(KMiddleRightMotorPort, vex::ratio6_1, KMiddleRightMotorRev), new vex::motor(KFrontRightMotorPort, vex::ratio6_1, KFrontRightMotorRev)};
Base robotBase(leftMotors, rightMotors);
PID turningPID(0.0, -0.45, 0.0, 0.0, 100, -100, 0.4);
PID movementPID(0.0, 3.4, 0.0, 0.0, 100, -100, 0.5);
Movement botMovement(&robotBase, true, true);

Controller mainController(vex::controllerType::primary);
vex::motor intakeMotor(KIntakeMotorPort, true); // rev so it starts the correct dir
vex::motor ladyBrownMotor(KWallStakeMotorPort, false);
vex::limit limitSwitch(botBrain.ThreeWirePort.B);
vex::rotation rotationSensor(KRotationSensorPort);
LadyBrown ladyBrown(&ladyBrownMotor,&mainController, &rotationSensor, &limitSwitch);
Hang botHangPneumatics;

AutonSelector autonSelector(mainController.getVexObject(), &botBrain);

vex::inertial* internalGyro = new vex::inertial(KInertialSensorPort);
Gyro* botGyro = new Gyro(internalGyro);
Odometry* botOdom = new Odometry(KOdomWheelSize, &robotBase, botGyro);
OdomMovement* gamer = new OdomMovement(botOdom, &botMovement, botGyro, &movementPID, &turningPID, &autonSelector);

Toggleable intakeEnabled;
Toggleable intakeReversed;

Toggleable mogoMechToggle;
vex::digital_out mogoMech(botBrain.ThreeWirePort.A);

bool preAutonReached = false;
uint64_t preAutonTime = 0;

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
  botBrain.Screen.pressing();
  //Tests if preauton is reach to make sure that we even reach preauton
  preAutonReached = true;
  preAutonTime = botBrain.Timer.systemHighResolution();
  std::cout << preAutonTime << std::endl; //standard time reahced = 0.37 seconds 
  std::cout << "  ^~~~~~~ That's the time ;DDDDDDDD" << std::endl;

  robotBase.resetAllEncoders();
  botGyro->resetGyroWithWait();

  autonSelector.add_auton("Do nothing");
  autonSelector.add_auton("Drive Forwards");
  autonSelector.add_auton("2 Stack AWP +0");
  autonSelector.add_auton("2 Stack +2");
  autonSelector.add_auton("2 Mogo +2");
  autonSelector.add_auton("testing");

  autonSelector.updateScreen();
  
  while(!Competition.isEnabled()) {
    autonSelector.update();
    vex::wait(5, vex::msec);
  }

  std::cout << botBrain.Timer.systemHighResolution() << std::endl; //standard time reahced = 0.37 seconds 
  std::cout << "  ^~~~~~~ That's the (end) time ;DDDDDDDD" << std::endl;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  // !! Reports of PreAuton not activating. Items need to be reset. !!
  // solutions so far: Failsafe in auton (takes 2 sec), 
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
  //pre auton failsafe
  if (!preAutonReached) {
    robotBase.resetAllEncoders();
    botGyro->resetGyroWithWait();
  }
  else {
    botBrain.Screen.printAt(0,0,"%lu", preAutonTime);
  }
  
  uint32_t setTime;
  movementPID.setBias(10.0);
  // int currentAuton = autonSelector.getCurrentAuton();
  // bool redAuton = autonSelector.getAutonRedSide();

  //temporary for testing specific auton
  autonSelector.setAuton(4);
  autonSelector.setAutonRedSide(true);

  // 0 = nothing
  // 1 = drive forwards
  // 2 = left to right AWP +0
  // 3 = left 2 + touching
  // 4 = right to left AWP + 0
  // 5 = right 2 + touching (slot 4)
  // 6 = left 4 + touching

  switch(autonSelector.getCurrentAuton()) {

    //do nothing
    case 0: {
      break;
    }

    //drive forwards
    case 1: {
      gamer->fixed(-15);
      break;
    }
    
    // left to right AWP +0
    case 2: {
      mogoMech.set(false);
      gamer->fixed(-35.0);
      mogoMech.set(true);
      // gamer->fixed(-2.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(315.0, 6.0, autonSelector.getAutonRedSide());
      mogoMech.set(false);
      gamer->fixed(17.5);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-46.0);
      gamer->turnToPosPIDSideFixed(45.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-16.971);
      mogoMech.set(true);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(40.0);
      break;
    }

    //left only *UNTESTED*
    case 3: {
      mogoMech.set(false);
      gamer->fixed(-35.0);
      mogoMech.set(true);
      // gamer->fixed(-2.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(40.0);
      break;
    }
    
    //right to left AWP +0
    case 4: {
      mogoMech.set(false);
      gamer->fixed(-35.0);
      mogoMech.set(true);
      // gamer->fixed(-2.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(45.0, 6.0, autonSelector.getAutonRedSide());
      mogoMech.set(false);
      gamer->fixed(17.5);
      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-46.0);
      gamer->turnToPosPIDSideFixed(315.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(-16.971);
      mogoMech.set(true);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(40.0);
      break;
    }
    //right 2 + touching
    case 5: {
      mogoMech.set(false);
      gamer->fixed(-35.0);
      mogoMech.set(true);
      // gamer->fixed(-2.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(24.0);
      setTime = vex::timer::system() + 2000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      gamer->turnToPosPIDSideFixed(90.0, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(40.0);
      break;
    }
    case 6: {
      mogoMech.set(false);
      gamer->fixedSlow(-35.0, 60);
      mogoMech.set(true);
      // gamer->fixed(-2.0);
      intakeMotor.spin(vex::forward, 50, vex::pct);
      setTime = vex::timer::system() + 500;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);

      gamer->turnToPosPIDSideFixed(90.00, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(30);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      gamer->turnToPosPIDSideFixed(171.31, 6.0, autonSelector.getAutonRedSide());
      gamer->fixed(15.8);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      intakeMotor.spin(vex::forward, 0, vex::pct);
      gamer->fixed(-15.8);

      gamer->turnToPosPIDSideFixed(189.69, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 50, vex::pct);
      gamer->fixed(15.8);
      setTime = vex::timer::system() + 1000;
      while (vex::timer::system() <= setTime) {vex::wait(5, vex::msec);}; // wait for score
      gamer->fixed(-15.8);

      gamer->turnToPosPIDSideFixed(270.0, 6.0, autonSelector.getAutonRedSide());
      intakeMotor.spin(vex::forward, 0, vex::pct);
      gamer->fixed(40.0);
      break;
    }
    // 
    case 7: {
      gamer->fixed(-(fieldTileLenIn - 6.0 - (botLengthIn/2.0)));
      gamer->turnToPosPID(-63.435, 6.0);
      
      ladyBrownMotor.resetPosition();
      ladyBrownMotor.spinToPosition(340.0, vex::deg, 100.0, vex::velocityUnits::pct, true);
      ladyBrownMotor.spinToPosition(0.0, vex::deg, 100.0, vex::velocityUnits::pct, true);

      mogoMech.set(false);
      gamer->fixed(40.249);
      mogoMech.set(true);

      gamer->turnToPosPID(180.0, 6.0);
      intakeMotor.spin(vex::forward, 100.0, vex::pct);
      gamer->fixed(-fieldTileLenIn);

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
      }
      else {
        intakeMotor.spin(vex::forward, 0, vex::pct);
      }

      mogoMech.set(mogoMechToggle.isEnabled());

      ladyBrown.run();

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