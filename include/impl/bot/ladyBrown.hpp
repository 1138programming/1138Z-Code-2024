#ifndef LADYBROWN_HPP
#define LADYBROWN_HPP

#include "../include/lib/resources/controller.hpp"
#include "../include/lib/resources/controller_button.hpp"
#include "../include/vex.h"
#include "../include/lib/resources/ladybrown_cycle.hpp"

#include <iostream>

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
            switch (cycle) {
                case DOWN:
                    cycle = LOADING;
                break;
                case LOADING:
                    cycle = OUT;
                break;
                case OUT:
                    cycle = DOWN;
                break;
            }
        }
        bool checkLastPressed(ControllerButton button) {
            bool* changeBool = NULL;
            bool buttonVal = this->vexController->getButton(button);
            bool retVal = false;

            switch(button) {
                case BUTTON_X:
                    changeBool = &xLastPressed;
                    retVal = (!xLastPressed && buttonVal);
                break;
                case BUTTON_A:
                    changeBool = &aLastPressed;
                    retVal = (!aLastPressed && buttonVal);
                break;
                case DPAD_UP:
                    changeBool = &upLastPressed;
                    retVal = (!upLastPressed && buttonVal);
                break;
                case DPAD_LEFT:
                    changeBool = &leftLastPressed;
                    retVal = (!leftLastPressed && buttonVal);
                break;
                case DPAD_DOWN:
                    changeBool = &downLastPressed;
                    retVal = (!downLastPressed && buttonVal);
                break;
            }
            (*changeBool) = buttonVal;
            return retVal;
        }
    public:
        LadyBrown(vex::motor* armMotor, Controller* vexController, vex::rotation* rotationSensor, vex::limit* limitSwitch) {
            this->vexController = vexController;
            this->armMotor = armMotor;
            this->armMotor->setStopping(vex::hold);
            this->rotationSensor = rotationSensor;
            this->limitSwitch = limitSwitch;
        }
        void startReset() {
            this->armMotor->spin(vex::forward, -75.0, vex::pct);
        }
        bool checkReset() {
            bool calibrated = this->limitSwitch->pressing();
            if (calibrated) {
                this->armMotor->resetPosition();
            }
            return calibrated;
        }
        void setResetted(bool reset) {
            if (reset) {
                this->cycle = DOWN;
            }
            else {
                this->cycle = START;
            }
        }
        void run() {
            bool xPressed = checkLastPressed(ControllerButton::BUTTON_X);
                if (xPressed) {
                    std::cout << "X pressed\n" << "\tnum: " << cycle << "\n";
                }
            bool upPressed = checkLastPressed(ControllerButton::DPAD_UP);
            bool aPressed = checkLastPressed(ControllerButton::BUTTON_A);
            //down (cycle #1)
            if (xPressed && cycle == OUT) {
                this->armMotor->spinToPosition(0, vex::degrees);
                cycleLadyBrown();
            }
            //loading (cycle #2)
            else if (xPressed && cycle == DOWN) {
                this->armMotor->spinToPosition(70, vex::degrees, 100.0, vex::velocityUnits::pct, false);
                cycleLadyBrown();
            }
            //wall stake (cycle #3)
            else if (xPressed && cycle == LOADING) {
                this->armMotor->spinToPosition(300, vex::degrees, 100.0, vex::velocityUnits::pct, false);
                cycleLadyBrown();
            }
            //alliance stake (separate button)
            else if (upPressed) {
                this->armMotor->spinToPosition(400, vex::degrees, 100.0, vex::velocityUnits::pct, false);
                cycle = OUT;
            }
            //manual loading
            else if (aPressed) {
                this->armMotor->spinToPosition(70, vex::degrees, 100.0, vex::velocityUnits::pct, false);
                cycle = LOADING;
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