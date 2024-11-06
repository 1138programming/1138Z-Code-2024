#ifndef ODOMETRY_HPP
#define ODOMETRY_HPP

#include "vector2.hpp"
#include "lib/bot/gyro.hpp"
#include "lib/bot/base.hpp"
#include "PID.hpp"

#define PI 3.1415926535897932384626

class Odometry {
    private:
        Gyro* gyro;
        Base* robotBase;

        Vector2 pos;
        float wheelDiam = 1.0; //default value
        float gearRatio = 0.75; //36::48

        double lastOdomPos;


        double convertDegToRad(double degrees) {
            return degrees * (PI/180.0);
        }
        double convertRadToDeg(double rad) {
            return rad * (180.0/PI);
        }


    public:
        bool doubleIsWithinMarginOfError(double num, double target, double margin) {
            double min = target - (margin / 2.0);
            double max = target + (margin / 2.0);
            return (num >= min && num <= max);
        }

        Odometry(float wheelDiameter, Base* robotBase, Gyro* gyro) {
            this->robotBase = robotBase;
            this->wheelDiam = wheelDiameter;
            this->gyro = gyro;
            // reset odom pos, motor encoders are absolute....
            this->robotBase->resetAllEncoders();
        }

        //getters
            double getX() {
                return this->pos.x;
            }
            double getY() {
                return this->pos.y;
            }
            Vector2 getPos() {
                return this->pos;
            }

        //setters
            void setX(double x) {
                this->pos.x = x;
            }
            void setY(double y) {
                this->pos.y = y;
            }
            void setPos(Vector2 pos) {
                this->pos = pos;
            }

        //useful funcs.
        void pollAndUpdateOdom() {
            double totalWheelMovement = this->robotBase->getAverageWheelRotationBothSides();
            // find averaged movement + convert to inches
            double averagedMovementDistance = getActualPosFromWheelRot(totalWheelMovement - this->lastOdomPos);

            // get heading + calculate values (in inches) from that
            double gyroHeading = this->gyro->getHeading();

            double xChange = this->xMult(gyroHeading);
            xChange *= averagedMovementDistance;

            double yChange = this->yMult(gyroHeading);
            yChange *= averagedMovementDistance;

            //--h+-+ello my name is noah bronsion
            // set last pos to use in next update
            this->pos.x += xChange;
            this->pos.y += yChange;
            this->lastOdomPos = totalWheelMovement;
        }
        double getActualPosFromWheelRot(double rot) {
            return rot * this->wheelDiam * PI * this->gearRatio;
        }

         double xMult(double deg) {
            return cos(this->convertDegToRad(deg));
        }
        double yMult(double deg) {
            return sin(this->convertDegToRad(deg));
        }
        double pythagoreanThrmBetweenTwoPoints(Vector2 pos1, Vector2 pos2) {
            double xChange = (pos1.x - pos2.x);
            double yChange = (pos1.y - pos2.y);

            double cSquared = (xChange * xChange) + (yChange * yChange);
            return sqrt(cSquared);
        }
        
        double signedPythagoreanThrmBetweenTwoPoints(Vector2 currentPos, Vector2 initialPos, Vector2 initialRiseOverRun, Vector2 targetPos, double margin) {
            // calculating sign
            // ignore if too close to 0...
            bool xDiff;
            bool yDiff;
            if (!doubleIsWithinMarginOfError(initialRiseOverRun.x, 0, margin)) {
                double newRun = (initialPos.x - currentPos.x);
                xDiff = ((newRun < 0) != (initialRiseOverRun.x < 0));
            }
            else {
                xDiff = true;
            }
            if (!doubleIsWithinMarginOfError(initialRiseOverRun.y, 0, margin)) {
                double newRise = (initialPos.y - currentPos.y);
                yDiff = ((newRise < 0) != (initialRiseOverRun.y < 0));
            }
            else {
                yDiff = true;
            }
            
            // calculating pythag. thrm.
            double xChange = (initialPos.x - currentPos.x);
            double yChange = (initialPos.y - currentPos.y);

            double cSquared = (xChange * xChange) + (yChange * yChange);

            return (yDiff && xDiff) ? -sqrt(cSquared) : sqrt(cSquared);
        }
};

#endif