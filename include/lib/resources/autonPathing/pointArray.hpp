#ifndef POINTARRAY_HPP
#define POINTARRAY_HPP

#include "point.hpp"
#include <vector>

class PointArray {
    private:
        std::vector<Point*> pointArr;
    public:
        /***
         * @warning It will handle memory management itself (use new())
         */
        void add(Point* point) {
            this->pointArr.push_back(point);
        }


        Point* at(int  loc) {
            return this->pointArr.at(loc);
        }

        int getSize() {
            return this->pointArr.size();
        }
};

#endif