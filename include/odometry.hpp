#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include "vex.h"
#include "gyro.hpp"
#include "base.hpp"

#define PI 3.14159265358979323846

class Odometry {
    
    float wheelDiameter = 0.0;
    float gearRatio = 1.0;
    double xPos = 0.0;
    double yPos = 0.0;
    double lastXChange = 0.0;
    double lastYChange = 0.0;
    double lastOdomPos;
    Base* robotBase;
    Gyro* gyro;
    PID* odomTurningPID;
    PID* odomMovementPID;
    //const Base* == the base is const; Base* const == the pointer is const; const Base* const == both.
    // the odometry class should NEVER change Base, but we can't garuentee that, as we have to access the functions in base. BE CAREFUL!
    private:
        double absD(double num) {
            return num < 0;
        }
        double convertDegToRad(double degrees) {
            return degrees * (PI/180);
        }
        double convertRadToDeg(double radians) {
            return radians * (180/PI);
        }
        bool doubleIsWithinMarginOfError(double num, double comparison, double marginOfError) {
            num = this->absD(num);
            comparison = this->absD(comparison);
            if (num > comparison - marginOfError && num < comparison + marginOfError) {
                return true;
            }
            return false;
        }
        double normalizeDegrees(double degrees) {
            double degreesNormalized = absD(fmod(degrees, 360));
            if (degreesNormalized < 0) {
                degreesNormalized +=360; // Convert the number to positive degrees so that -1 is actually equal to 359 (considering we're using cos now, this is probably an unnessacary step.)
            }
        }
        double pythagoreanTheormBetweenTwoPoints(double x1, double y1, double x2, double y2) {
            double xChange = (x1-x2);
            double yChange = (y1-y2);
            double cSquared = (xChange * xChange) + (yChange * yChange);
            double c = sqrt(cSquared);
            return c;
        }
    public:
        Odometry(float wheelDiameter, Base* robotBase, Gyro* gyro) {
            this->robotBase = robotBase;
            this->wheelDiameter = wheelDiameter;
            this->gyro = gyro;
        }
        Odometry(float wheelDiameter, float gearRatio, Base* robotBase, Gyro* gyro, PID* turningPID, PID* movementPID) {
            this->robotBase = robotBase;
            this->wheelDiameter = wheelDiameter;
            this->gearRatio = gearRatio;
            this->gyro = gyro;
            this->odomTurningPID = turningPID;
            this->odomMovementPID = movementPID;
        }
        double getX() {
            return this->xPos;
        }
        void setX(double x) {
            this->xPos = x;
        }
        double getY() {
            return this->yPos;
        }
        void setY(double y) {
            this->yPos = y;
        }
        double getLastXChange() {
            return this->lastXChange;
        }
        double getLastYChange() {
            return this->lastYChange;
        }
        // ignore this comment unless multitasking... alright, so the reason it is static is genuinely very interesting. if you wanna learn more, read this: https://stackoverflow.com/questions/400257/how-can-i-pass-a-class-member-function-as-a-callback
        void pollAndUpdateOdom() {
            double averagedMovementDifference = this->robotBase->getAverageRotationBothSides()-(this->lastOdomPos);
            //get the change in inches rather than encoder units (hopefully)
            averagedMovementDifference = this->getActualPosFromWheelRot(averagedMovementDifference);

            double gyroHeading = this->gyro->getHeading();

            double xChange = this->getXPosMultFromDegreesInRad(gyroHeading);
            xChange *= averagedMovementDifference;

            double yChange = this->getYPosMultFromDegInRad(gyroHeading);
            yChange *= averagedMovementDifference;

            // update the X and Y positions
            this->xPos += xChange;
            this->yPos += yChange;
            // for testing
            this->lastXChange = xChange;
            this->lastYChange = yChange;
            //--h+-+ello my name is noah bronsion
            //set the last pos so we can use it in the future
            this->lastOdomPos = this->robotBase->getAverageRotationBothSides();
        }
        double getXPosMultFromDegreesInRad(double gyroDegrees) {
            // since this is heading, it shouldn't matter, but incase I want to use raw values.
            //double degreesNormalized = normalizeDegrees(gyroDegrees);
            // cos() takes a value in radians, so we have to convert to them.
            return (cos(convertDegToRad(gyroDegrees)));
        }
        double getYPosMultFromDegInRad(double gyroDegrees) {
            //double degreesNormalized = normalizeDegrees(gyroDegrees);
            // sin() takes a value in radians, so we have to convert to them.
            return (sin(convertDegToRad(gyroDegrees)));
        }
        double getActualPosFromWheelRot(double rot) {
            return rot * this->wheelDiameter * this->gearRatio;
        }
        void moveForwardToPosInInches(double pos, int speed) {
            float totalAverageRotationInches = this->getY();
            float startRot = totalAverageRotationInches;

            while (totalAverageRotationInches - startRot != pos) {
                this->robotBase->driveBothSides(speed);
                vex::wait(10,vex::msec);
                totalAverageRotationInches = this->getY();
            }
            this->robotBase->driveBothSides(0);
        }
        void turnToPos(double targetPosition) {
            this->gyro->resetGyro();
            //!(this->doubleIsWithinMarginOfError(this->gyro->getRot(),targetPosition,1))
            while (!(this->doubleIsWithinMarginOfError(this->gyro->getHeading(),targetPosition,10))) {
                vex::wait(10,vex::msec);
                if (this->gyro->getHeading() > targetPosition) {
                    (*this->robotBase).turn(50);
                }
                else {
                    (*this->robotBase).turn(-50);
                }
            }
            this->robotBase->stop();
        }
        void turnToPosPID(double targetPosition, double allowedError) {
            this->odomTurningPID->setSetpoint(targetPosition);
            //!(this->doubleIsWithinMarginOfError(this->gyro->getRot(),targetPosition,1))
            while (!(this->doubleIsWithinMarginOfError(this->gyro->getHeading(),targetPosition,allowedError))) {
                vex::wait(10,vex::msec);
                double PIDVal = this->odomTurningPID->calculate(this->gyro->getHeading());
                this->robotBase->turn(PIDVal);
            }
            this->robotBase->stop();
        }
        void moveInInchesOdom(double inches, double allowedError) {
            double gyroHeader = this->gyro->getHeading();
            double xMultiplier = this->getXPosMultFromDegreesInRad(gyroHeader);
            double yMultiplier = this->getYPosMultFromDegInRad(gyroHeader);
            double posToMoveToX = (this->xPos + (this->getActualPosFromWheelRot(inches) * xMultiplier));
            double posToMoveToY = (this->yPos + (this->getActualPosFromWheelRot(inches) * yMultiplier));
            double initialPosX = this->xPos;
            double initialPosY = this->yPos;

            double initialDisplacement = this->pythagoreanTheormBetweenTwoPoints(initialPosX, initialPosY, posToMoveToX, posToMoveToY);
            //this->doubleIsWithinMarginOfError()
            // don't bother trying to read this, takes the most different pos and does the loop while that happens.
            while(this->doubleIsWithinMarginOfError((this->pythagoreanTheormBetweenTwoPoints(this->xPos, this->yPos, initialPosX, initialPosY)-initialDisplacement), 0.0, allowedError)) { //!(this->doubleIsWithinMarginOfError(this->xPos, posToMoveToX, allowedError))) { //!(xDifferenceGreater ? this->doubleIsWithinMarginOfError(this->xPos, posToMoveToX, allowedError) : this->doubleIsWithinMarginOfError(this->yPos, posToMoveToY, allowedError))) {
                double difference = (this->pythagoreanTheormBetweenTwoPoints(this->xPos, this->yPos, initialPosX, initialPosX))-initialDisplacement;
                if (difference >= 0) {
                    // greater than or equal to
                    this->robotBase->driveBothSides(20);
                }
                else {
                    this->robotBase->driveBothSides(-20);
                }
                vex::wait(10,vex::msec);
                this->pollAndUpdateOdom();
            }
        }
        // TBD
        void moveInInchesOdomPID(double inches, double allowedError) {
            double gyroHeader = this->gyro->getHeading();

            double xMultiplier = this->getXPosMultFromDegreesInRad(gyroHeader);
            double yMultiplier = this->getYPosMultFromDegInRad(gyroHeader);

            double posToMoveToX = (this->xPos + (inches * xMultiplier));
            double posToMoveToY = (this->yPos + (inches * yMultiplier));

            double initialPosX = this->xPos;
            double initialPosY = this->yPos;

            double initialDisplacement = this->pythagoreanTheormBetweenTwoPoints(initialPosX, initialPosY, posToMoveToX, posToMoveToY);
            double movement = -0.001;
            
            if (initialDisplacement == inches) {
                return;
            }

            this->odomMovementPID->setSetpoint(0.0);
            while (movement != 0.0) {
                double difference = (this->pythagoreanTheormBetweenTwoPoints(this->xPos, this->yPos, initialPosX, initialPosY));
                movement = this->odomMovementPID->calculate(inches - difference);
                this->robotBase->driveBothSides(movement);
                vex::wait(10,vex::msec);
                this->pollAndUpdateOdom();
            }
        }
};

#endif