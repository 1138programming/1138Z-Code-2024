#ifndef TOGGLEABLE_HPP
#define TOGGLEABLE_HPP

class Toggleable {
    protected:
        bool enabled;
        bool pressed;
        bool lastPressed;
    public:
        void setEnabled() {
            this->enabled = true;
        }
        void setDisabled() {
            this->enabled = false;
        }
        void toggle() {
            this->enabled = !(this->enabled);
        }

        // TODO: need???? - yes???????? (what was past me cooking?)
        void update(bool updateVal) {
            this->lastPressed = this->pressed;
            this->pressed = updateVal;
            if (this->isPressed()) {
                this->toggle();
            }
        }

        bool isPressed() {
            return (this->lastPressed == false && this->pressed == true);
        }

        bool isEnabled() {
            return this->enabled;
        }
};

#endif