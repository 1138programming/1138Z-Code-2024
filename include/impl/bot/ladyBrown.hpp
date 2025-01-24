#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP

#include "../include/lib/resources/controller.hpp"
#include "../include/lib/resources/controller_button.hpp"
#include "../include/vex.h"
#include "../include/lib/resources/ladybrown_cycle.hpp"

class LadyBrown {
    private:
        Controller* vexController;
        vex::motor* armMotor;
        vex::limit* limitSwitch;
        vex::rotation* rotationSensor;
        LadyBrownCycle cycle = START;
        bool resetting = false;
        bool xLastPressed = false;
        bool aLastPressed = false;
        bool upLastPressed = false;
        bool leftLastPressed = false;
        bool downLastPressed = false;
        
        void normalizeArmPosition() {
            while(!limitSwitch->PRESSED) {
                this->armMotor->spin(vex::forward, 20, vex::pct);
            }
            this->armMotor->spin(vex::forward, 0, vex::pct);
            rotationSensor->resetPosition();
        }
        void cycleLadyBrown() {
            int cycleNum = static_cast<int>(this->cycle);
            cycleNum++;
            if (cycleNum >= RESETVAL) {
                cycleNum = 1;
            }
            this->cycle = static_cast<LadyBrownCycle>(cycleNum);
        }
        bool checkLastPressed(ControllerButton button) {
            switch(button) {
                case BUTTON_X:
                    if(xLastPressed) return false;
                    xLastPressed = true;
                break;
                case BUTTON_A:
                    if(aLastPressed) return false;
                    aLastPressed = true;
                break;
                case DPAD_UP:
                    if(upLastPressed) return false;
                    upLastPressed = true;
                break;
                case DPAD_LEFT:
                    if(leftLastPressed) return false;
                    leftLastPressed = true;
                break;
                case DPAD_DOWN:
                    if(downLastPressed) return false;
                    downLastPressed = true;
                break;
            }
            return true;
        }
    public:
        LadyBrown(vex::motor* armMotor, Controller* vexController, vex::rotation* rotationSensor, vex::limit* limitSwitch) {
            this->vexController = vexController;
            this->armMotor = armMotor;
            this->armMotor->setStopping(vex::hold);
            this->rotationSensor = rotationSensor;
            this->limitSwitch = limitSwitch;

            //this->vexController->getVexObject()->ButtonA.released();
        }
        void run() {
            //down (cycle #1)
            if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == OUT) {
                if(checkLastPressed(ControllerButton::BUTTON_X)){
                this->armMotor->spinToPosition(0, vex::degrees);
                cycleLadyBrown();
                }
            }
            //loading (cycle #2)
            else if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == DOWN) {
                if(checkLastPressed(ControllerButton::BUTTON_X)){
                    this->armMotor->spinToPosition(-60, vex::degrees, false);
                    cycleLadyBrown();
                }
            }
            //wall stake (cycle #3)
            else if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == LOADING) {
                if(checkLastPressed(ControllerButton::BUTTON_X)){
                    this->armMotor->spinToPosition(-300, vex::degrees, false);
                    cycleLadyBrown();
                }
            }
            //alliance stake (separate button)
            else if (this->vexController->getVexObject()->ButtonUp.PRESSED) {
                if(checkLastPressed(ControllerButton::DPAD_UP)){
                    this->armMotor->spinToPosition(-400, vex::degrees, false);
                    cycle = OUT;
                }
            }
            //manual loading
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                if(checkLastPressed(ControllerButton::BUTTON_A)){
                    this->armMotor->spinToPosition(-60, vex::degrees, false);
                    cycle = LOADING;
                }
            }
            //manual scoring
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                if(checkLastPressed(ControllerButton::BUTTON_X)){
                    this->armMotor->spin(vex::forward, 40, vex::pct);
                    cycle = DOWN;
                }
            }
            //manual down
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                if(checkLastPressed(ControllerButton::BUTTON_X)){
                    this->armMotor->spin(vex::forward, 40, vex::pct);
                    cycle = DOWN;
                }
            }
            //manual reset
            else if (this->vexController->getVexObject()->ButtonDown.PRESSED) {
                if(checkLastPressed(ControllerButton::DPAD_DOWN)){
                    this->armMotor->spin(vex::forward, 40, vex::pct);
                    cycle = DOWN;
                }
            }
        }
};

#endif