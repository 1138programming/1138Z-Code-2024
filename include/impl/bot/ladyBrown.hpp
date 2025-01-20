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
    public:
        LadyBrown(vex::motor* armMotor, Controller* vexControoller, vex::rotation* rotationSensor, vex::limit* limitSwitch) {
            this->vexController = vexController;
            this->armMotor = armMotor;
            this->armMotor->setStopping(vex::hold);
            this->rotationSensor = rotationSensor;
            this->limitSwitch = limitSwitch;
        }
        void normalizeArmPosition() {
            while(!limitSwitch->PRESSED) {
                this->armMotor->spin(vex::forward, 20, vex::pct);
            }
            this->armMotor->spin(vex::forward, 0, vex::pct);
            rotationSensor->resetPosition();
        }
        void cycleLadyBrown() {
            switch(cycle) {
                case START:
                cycle = DOWN;
                break;
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
        void run() {
            //down (cycle #1)
            if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == OUT) {
                this->armMotor->spinToPosition(0, vex::degrees, false);
                cycleLadyBrown();
            }
            //loading (cycle #2)
            else if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == DOWN) {
                this->armMotor->spinToPosition(-60, vex::degrees, false);
                cycleLadyBrown();
            }
            //wall stake (cycle #3)
            else if (this->vexController->getVexObject()->ButtonX.PRESSED && cycle == LOADING) {
                this->armMotor->spinToPosition(-300, vex::degrees, false);
                cycleLadyBrown();
                
            }
            //alliance stake (separate button)
            else if (this->vexController->getVexObject()->ButtonUp.PRESSED) {
                this->armMotor->spinToPosition(-400, vex::degrees, false);
                cycle = OUT;
            }
            //manual loading
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                this->armMotor->spinToPosition(-60, vex::degrees, false);
                cycle = LOADING;
            }
            //manual scoring
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                this->armMotor->spin(vex::forward, 40, vex::pct);
                cycle = DOWN;
            }
            //manual down
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                this->armMotor->spin(vex::forward, 40, vex::pct);
                cycle = DOWN;
            }
            //reset
            else if (this->vexController->getVexObject()->ButtonA.PRESSED) {
                this->armMotor->spin(vex::forward, 40, vex::pct);
                cycle = DOWN;
            }
            else {
                this->armMotor->spin(vex::forward, 0, vex::pct);
            }
        }
};

#endif