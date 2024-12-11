#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#include "point.hpp"
#include "dataPointType.hpp"

class EndPoint : public Point {
    public:
        float angle;

        EndPoint(DataPointType pointType) : Point(pointType) {}
        EndPoint(DataPointType pointType, std::vector<char>& data, int index) : Point(pointType) {
            this->readToClass(data, index);
        }
        
        virtual int readToClass(std::vector<char>& vec, int index) {
            readToVar(&this->angle, sizeof(float), vec, index);
                index += sizeof(float);
            
            readToVar(&this->pointPlacement.x, sizeof(float), vec, index);
                index += sizeof(float);
            readToVar(&this->pointPlacement.y, sizeof(float), vec, index);
                index += sizeof(float);
            return index;
        }
};

#endif