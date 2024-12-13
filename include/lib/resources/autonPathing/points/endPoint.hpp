#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#include "point.hpp"
#include "dataPointType.hpp"

class EndPoint : public Point {
    public:
        float angle;

        EndPoint(DataPointType pointType) : Point(pointType) {}
        /***
         * @warning PLEASE MAKE SURE THIS DOES WHAT YOU EXPECT. It auto-increments index. Manually use the readToClass(std::vector<char>& vec, int index) function if you don't want this behaviour.
         */
        EndPoint(DataPointType pointType, std::vector<char>& data, int& index) : Point(pointType) {
            index += this->readToClass(data, index);
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