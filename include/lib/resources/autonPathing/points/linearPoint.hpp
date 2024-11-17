#ifndef LINEARPOINT_HPP
#define LINEARPOINT_HPP

#include "point.hpp"

class LinearPoint : public Point {
    private:
        bool isBackwards;
    public:
        virtual void readToClass(std::vector<char>& vec, int index) {
            readToVar(&this->isBackwards, sizeof(bool), vec, index)
            
            readToVar(&this->hipointPlacement.x, sizeof(float), vec, index);
            readToVar(&this->pointPlacement.y, sizeof(float), vex, index);
        }
};

#endif