#ifndef ODOMMOVEMENT_HPP
#define ODOMMOVEMENT_HPP

#include "lib/resources/odometry.hpp"
#include "movement.hpp"

class OdomMovement {
    private:
        Odometry* odom;
        Movement* robotMovement;
        Gyro* gyro;

        PID* odomTurningPID;
        PID* odomMovementPID;

        double inchesMult = 1.0;

        double convertNumToDeg(double num) {
            return fmod(num, 360.0);
        }

        double getDistBetweenDeg(double botDeg, double targetDeg) {
            // but: https://stackoverflow.com/questions/9505862/shortest-distance-between-two-degree-marks-on-a-circle
            float diff, sweepAngle;

            diff = botDeg - targetDeg;

            if (diff < -180) {
                sweepAngle = -(360+diff);
            }
            else if (diff > 180) {
                sweepAngle = 360-diff;
            }
            else {
                sweepAngle = -diff;
            }
            return sweepAngle;
        }
    public:
        OdomMovement(Odometry* odom, Movement* robotMovement, Gyro* gyro, PID* odomMovementPID, PID* odomTurningPID) {
            this->odom = odom;
            this->robotMovement = robotMovement;
            this->gyro = gyro;

            this->odomMovementPID = odomMovementPID;
            this->odomTurningPID = odomTurningPID;
        }

        //calc is short for calculation
        double angleSideCalc(double angle, bool redSide) {
            if(!redSide) {
                return (360.0 - angle);
            }
            return angle;
        }

        void turnToPosPID(double targetPos, double allowedError) {
            vex::controller cont(vex::controllerType::primary);
            // make 0s            
            this->odomTurningPID->setSetpoint(0.0);
            this->odomTurningPID->setAllowedError(allowedError);

            while(this->odomTurningPID->isPidFinished() == false) {
                cont.Screen.clearLine(0);
                cont.Screen.setCursor(0,0);

                this->odom->pollAndUpdateOdom();
                double PIDVal = this->odomTurningPID->calculate(getDistBetweenDeg(this->gyro->getHeading(), targetPos));
                
                cont.Screen.print("%f, %f", PIDVal,/*getDistBetweenDeg(this->gyro->getHeading(), targetPos),*/ this->gyro->getHeading());
                this->robotMovement->turn(PIDVal);
                // wait for bot to move
                vex::wait(5, vex::msec);
            }
        }
        void moveInInchesOdomPID(double inches) {
            bool rev = false;
            if (inches < 0) {
                rev = true;
            }
            inches *= this->inchesMult;
            double initialGyroHeader = this->gyro->getHeading();

            double xMult = this->odom->xMult(initialGyroHeader);
            double yMult = this->odom->xMult(initialGyroHeader);

            double posToMoveToX = (this->odom->getX() + (inches * xMult));
            double posToMoveToY = (this->odom->getY() + (inches * yMult));

            double initialPosX = this->odom->getX();
            double initialPosY = this->odom->getY();

            Vector2 initialRiseOverRun = (Vector2){posToMoveToY-initialPosY,posToMoveToX-initialPosX};

            double initialDisplacement = this->odom->pythagoreanThrmBetweenTwoPoints((Vector2){initialPosX, initialPosY}, (Vector2){posToMoveToX, posToMoveToY});
            double movement = 20;

            this->odomMovementPID->setSetpoint(0.0);
            do {
                vex::wait(5, vex::msec);
                this->odom->pollAndUpdateOdom();
                double difference = (this->odom->signedPythagoreanThrmBetweenTwoPoints((Vector2) {this->odom->getX(), this->odom->getY()}, (Vector2) {initialPosX, initialPosY}, initialRiseOverRun, (Vector2){posToMoveToX, posToMoveToY}, 0.1));
                movement = this->odomMovementPID->calculate(inches - difference);
                if (rev) {
                    this->robotMovement->robotBase->driveBothSides((int)-movement);
                }
                else {
                    this->robotMovement->robotBase->driveBothSides((int)movement);
                }
            }
            while (this->odomMovementPID->isPidFinished() == false);
        }
        void fixed(double inches) {
            inches *= kInchesFixMult;

            bool negative = false;
            if (inches < 0) {
                negative = true;
            }
            // get vector2s
            this->odom->pollAndUpdateOdom();
            Vector2 initialPos = this->odom->getPos();
            // set up PID
            this->odomMovementPID->setSetpoint(0.0);
            this->odomMovementPID->setAllowedError(0.02);

            vex::controller cont(vex::controllerType::primary);

            do {
                // controller stuff
                cont.Screen.clearLine(0);
                vex::wait(5, vex::msec);
                cont.Screen.setCursor(0,0);

                this->odom->pollAndUpdateOdom();

                Vector2 currentPos = this->odom->getPos();
                
                double dif = this->odom->pythagoreanThrmBetweenTwoPoints(initialPos, currentPos);
                cont.Screen.print("%f, %f", this->odom->getX(), this->odom->getY());
                // cont.Screen.print("%lf", dif);

                double movement;
                if (negative) {
                    movement = this->odomMovementPID->calculate(inches + dif);
                }
                else {
                    movement = this->odomMovementPID->calculate(inches - dif);
                }
                this->robotMovement->robotBase->driveBothSides(movement);

            } while(this->odomMovementPID->isPidFinished() == false);
        }
};

#endif