#ifndef POINTINITIAL_HPP
#define POINTINITIAL_HPP

#include "point.hpp"
#include <vector>

class PointInitial : public Point {
    public:
        bool isBlue;
        float angle;

        virtual int readToClass(std::vector<char>& vec, int index) {
            this->readToVar(&this->isBlue, sizeof(bool), vec, index);
                index += sizeof(bool);
            
            this->readToVar(&this->angle, sizeof(float), vec, index);
                index += sizeof(float);

            this->readToVar(&this->pointPlacement.x, sizeof(float), vec, index);
                index += sizeof(float);
            this->readToVar(&this->pointPlacement.y, sizeof(float), vec, index);
                index += sizeof(float);
            return index;
        }
};

#endif