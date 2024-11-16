#ifndef AUTONPATHPARSER_HPP
#define AUTONPATHPARSER_HPP

#include <vector>
#include "../commands/odomMovement.hpp"
#include "autonVersioningUnion.hpp"

class autonPathParser {
private:
    std::vector<char> autonPathData;
    OdomMovement* odomMovement;

    void init() {
        
    }
public:
    autonPathParser(char* autonPathData, int dataSize, OdomMovement* odomMovement) {
        this->odomMovement = odomMovement;
        this->autonPathData.reserve(dataSize);

        for(int i = 0; i < dataSize; i++) {
            this->autonPathData.push_back(autonPathData[i]);
        }
    }

    void update() {

    }
};



#endif