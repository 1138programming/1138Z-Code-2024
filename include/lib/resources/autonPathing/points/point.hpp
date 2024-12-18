#ifndef POINT_HPP
#define POINT_HPP

#include "dataPointType.hpp"
#include "../vector2f.hpp"

#include <vector>

class Point {
    protected:
        DataPointType pointType;
        Vector2F pointPlacement;

        void readToVar(void* var, int varSize, std::vector<char>& vec, int indexOffset) {
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

        virtual int readToClass(std::vector<char>& vec, int index) {
            readToVar(&this->pointPlacement.x, sizeof(float), vec, index);
                index += sizeof(float);
            readToVar(&this->pointPlacement.y, sizeof(float), vec, index);
                index += sizeof(float);
            return index;
        }

        DataPointType getPointType() {
            return this->pointType;
        }
        Vector2F getPointPlacement() {
            return this->pointPlacement;
        }

        // needed to make child class pointers work
        virtual ~Point() {}
};

#endif