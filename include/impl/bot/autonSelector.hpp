#ifndef AUTONSELECTOR_HPP
#define AUTONSELECTOR_HPP

#include "vex.h"
#include <vector>
#include <string>
#include <iostream>

class AutonSelector {
    private:
        vex::controller* controller;
        vex::brain* limitSwitchSel;
        std::vector<std::string> autonList;
        int currentAuton = 0;
        bool redAuton = true;
        bool scrollLastPressed;
    public:
        AutonSelector(vex::controller* controller, vex::brain* limitSwitchSelector) {
            this->controller = controller;
            this->limitSwitchSel = limitSwitchSelector;
        }

        void add_auton(std::string auton) {
            this->autonList.push_back(auton);
            std::cout << auton << std::endl;
        }

        void update() {
            if (this->autonList.size() == 0) {
                return;
            }
            bool numChanged = false;
            
            bool scrollPressed = this->limitSwitchSel->Screen.pressing();

            if (!this->scrollLastPressed && scrollPressed) {
                this->currentAuton++;
                numChanged = true;
            }

            if (this->currentAuton > this->autonList.size()-1) {
                this->currentAuton = 0;
                this->redAuton = !this->redAuton;
            }

            if (numChanged) {
                updateScreen();
            }

            this->scrollLastPressed = scrollPressed;
        }

        void updateScreen() {
            this->controller->Screen.setCursor(0,0);
            this->controller->Screen.clearLine();
            std::string fullStr = (this->redAuton ? "Red: " : "Blue: ") + this->autonList.at(this->currentAuton);
            this->controller->Screen.print(fullStr.c_str());
        }

        int getCurrentAuton() {
            return this->currentAuton;
        }
        bool getAutonRedSide() {
            return this->redAuton;
        }

        void setAuton(int autonNum) {
            this->currentAuton = autonNum;
        }
        void setAutonRedSide(bool redSide) {
            this->redAuton = redSide;
        }

        double getFixedAngle(double angle) {
            if (!this->redAuton) {
                return (360.0 - angle);
            }
            return angle;
        }
};


#endif