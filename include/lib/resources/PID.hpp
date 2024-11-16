#ifndef PID_HPP
#define PID_HPP

#include <cstdint>
#include "vex.h"

/**
 * @breif Blind recreation of prev. class written...
 * TODO - Make sure code is actually correct (study PIDs)
*/
class PID {
    private:
        double setpoint;

        double Kp;
        double Ki;
        double Kd;

        double error;
        double prevIntegral;
        double prevError;

        double outputMin;
        double outputMax;

        double bias = 0.0;
        double allowedError = 0.1;

        bool isFinished = false;

        std::uint32_t iterationTime = 0;
        std::uint32_t lastMillis = 1;
        bool doubleIsWithinMarginOfError(double num, double target, double margin) {
            double min = target - (margin / 2.0);
            double max = target + (margin / 2.0);
            return (num >= min && num <= max);
        }
    public:
        PID(double setpoint, double Kp, double Ki, double Kd, double outputMax, double outputMin, double allowedError) {
            this->setpoint = setpoint;
            
            this->Kp = Kp;
            this->Ki = Ki;
            this->Kd = Kd;

            this->outputMax = outputMax;
            this->outputMin = outputMin;

            this->allowedError = allowedError;
            this->lastMillis = vex::timer::system();
            this->isFinished = false;
        }

        // setters
        void setSetpoint(double setpoint) {
            this->setpoint = setpoint;
            this->isFinished = false;
        }
        void setAllowedError(double allowedError) {
            this->allowedError = allowedError;
        }

        // getters
        bool isPidFinished() {
            return this->isFinished;
        }

        //useful funcs
        double calculate(double processVar) {
            // checks if we should ret 0
            if (this->isFinished) {
                return 0.0;
            }
            if (doubleIsWithinMarginOfError(processVar, this->setpoint, this->allowedError)) {
                this->isFinished = true;
                return 0.0;
            }

            this->iterationTime = vex::timer::system() - lastMillis;

            // calculate vars
            this->error = setpoint - processVar;
            double integral = prevIntegral + error * iterationTime;
            double derivative = (error - prevError) / iterationTime;

            double output = (this->Kp * error) + (this->Ki * integral) + (this->Kd * derivative) + (bias);

            // set vars up for error   
            this->prevError = error;
            this->prevIntegral = integral;
            this->lastMillis = vex::timer::system();
            
            // check output to make sure if it is within bounds
            // TODO: add warnings if output max / min is constantly hit
            if (output > this->outputMax) {
                return this->outputMax;
            }
            else if (output < this->outputMin) {
                return this->outputMin;
            }

            return output;
        }
};

#endif