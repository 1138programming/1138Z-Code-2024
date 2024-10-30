#ifndef BASE_HPP
#define BASE_HPP

#include "lib/resources/motorgroup.hpp"
#include "vex.h"

class Base {
    private:
        MotorGroup left;
        MotorGroup right;
        bool reversed;
    public:
        // ___ Constructor ___
        /**
         * @brief Creates new base and sets motors to brake (trust)
        */
        Base(MotorGroup leftSide, MotorGroup rightSide) {
            this->left = leftSide;
            this->right = rightSide;
            this->left.setBrakeMode(vex::brakeType::brake);
            this->right.setBrakeMode(vex::brakeType::brake);
        }

        // ___ Useful Functions ___
        void driveBothSides(int movement) {
            this->left.move(movement);
            this->right.move(movement);
        }
        void moveLeftMotors(int movement) {
            this->left.move(movement);
        }
        void moveRightMotors(int movement) {
            this->right.move(movement);
        }
        void stop() {
            this->left.stop();
            this->right.stop();
        }

        void resetAllEncoders() {
            this->left.resetEncoders();
            this->right.resetEncoders();
        }
        double getAverageWheelRotationBothSides() {
            return ((this->left.averageRot(vex::rotationUnits::deg) + this->right.averageRot(vex::rotationUnits::deg))/2.0)/360.0;
        }
        
};

#endif