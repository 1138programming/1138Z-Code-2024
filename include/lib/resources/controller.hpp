#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "vex.h"
#include "controller_axis.hpp"
#include "controller_button.hpp"

class Controller {
    private:
        vex::controller* internalController;

        static void testFunctionCallback(void* testArg) {

        }
    public:
        Controller(vex::controllerType type) {
            this->internalController = new vex::controller(type);
        }
        /**
         * @brief Allows users to do whatever they want with the internal controller
        */
        vex::controller* getVexObject() {
            return this->internalController;
        }

        //functions
        /**
         * @brief returns a value between -100 and 100 representing the joystick axis position
         * @param axis The vex controller axis to use (as depicted by the buttons on this image): https://www.vexrobotics.com/media/catalog/product/cache/d64bdfbef0647162ce6500508a887a85/2/7/276-4820-v5-controller_home.jpg
         * @return value between -100 and 100 representing the joystick axis position
         * @warning vex controllers have pretty terrible deadzones... don't expect small numbers to be detected
        */
        int getAxis(ControllerAxis axis) {
            switch(axis) {
                case AXIS_1:
                    return this->internalController->Axis1.position();
                break;
                case AXIS_2:
                    return this->internalController->Axis2.position();
                break;
                case AXIS_3:
                    return this->internalController->Axis3.position();
                break;
                case AXIS_4:
                    return this->internalController->Axis4.position();
                break;
                default:
                    return -1;
            }
        }
        bool getButton(ControllerButton button) {
            switch (button) {
                case BUTTON_A:
                    return this->internalController->ButtonA.pressing();
                    break;
                case BUTTON_B:
                    return this->internalController->ButtonB.pressing();
                    break;
                case BUTTON_X:
                    return this->internalController->ButtonX.pressing();
                    break; 
                case BUTTON_Y:
                    return this->internalController->ButtonY.pressing();
                    break;
                case DPAD_UP:
                    return this->internalController->ButtonUp.pressing();
                    break;
                case DPAD_RIGHT:
                    return this->internalController->ButtonRight.pressing();
                    break;
                case DPAD_DOWN:
                    return this->internalController->ButtonDown.pressing();
                    break;
                case DPAD_LEFT:
                    return this->internalController->ButtonLeft.pressing();
                    break;
                case BUTTON_L1:
                    return this->internalController->ButtonL1.pressing();
                    break;
                case BUTTON_R1:
                    return this->internalController->ButtonR1.pressing();
                    break;
                case BUTTON_L2:
                    return this->internalController->ButtonL2.pressing();
                    break;
                case BUTTON_R2:
                    return this->internalController->ButtonR2.pressing();
                    break;
                default:
                    return false;
            }    
        }
};

#endif