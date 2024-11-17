#ifndef POINT_HPP
#define POINT_HPP

#include "dataPointType.hpp"
#include "../vector2f.hpp"

#include <vector>

class Point {
    protected:
        DataPointType pointType;
        Vector2F pointPlacement;

        readToVar(void* var, int varSize, std::vector<char>& vec, int indexOffset) {
            for (int i = 0; i < varSize; i++) {
                ((char*)var)[i] = vec.at(indexOffset + i);
            }
        }
    public:
        Point(DataPointType pointType) {
            this->pointType = pointType;
        }
        Point(DataPointType pointType, std::vector<char>& data, int index) {
            this->pointType = pointType;
            this->readToClass(data, index);
        }

        virtual void readToClass(std::vector<char>& vec, int index) {
            readToVar(&this->hipointPlacement.x, sizeof(float), vec, index);
            readToVar(&this->pointPlacement.y, sizeof(float), vex, index);
        }
};

#endif