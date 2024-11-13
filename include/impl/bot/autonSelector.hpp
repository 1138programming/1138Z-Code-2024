#ifndef AUTONSELECTOR_HPP
#define AUTONSELECTOR_HPP

#include "vex.h"
#include <vector>
#include <string>

class AutonSelector {
    private:
        vex::controller* controller;
        std::vector<std::string> autonList;
        int currentAuton;
        bool leftLastPressed, rightLastPressed;
    public:
        AutonSelector(vex::controller* controller) {
            this->controller = controller;
        }

        void add_auton(std::string auton) {
            this->autonList.push_back(auton);
        }

        void update() {
            if (this->autonList.size() == 0) {
                return;
            }
            bool numChanged = false;

            bool leftPressed = this->controller->ButtonLeft.pressing() || (this->controller->Axis4.position() > 90);
            bool rightPressed = this->controller->ButtonRight.pressing() || (this->controller->Axis4.position() < -90);

            if (!leftLastPressed && leftPressed) {
                this->currentAuton--;
                numChanged = true;
            }
            if (!rightLastPressed && rightPressed) {
                this->currentAuton++;
                numChanged = true;
            }

            if (this->currentAuton < 0) {
                this->currentAuton = this->autonList.size()-1;
            }
            if (this->currentAuton > this->autonList.size()-1) {
                this->currentAuton = 0;
            }

            if (numChanged) {
                updateScreen();
            }

            this->leftLastPressed = leftPressed;
            this->rightLastPressed = rightPressed;
        }

        void updateScreen() {
            this->controller->Screen.setCursor(0,0);
            this->controller->Screen.clearLine();
            this->controller->Screen.print(this->autonList.at(this->currentAuton).c_str());
        }

        int getCurrentAuton() {
            return this->currentAuton;
        }

};


#endif