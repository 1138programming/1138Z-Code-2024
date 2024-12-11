#ifndef AUTONPATHPARSER_HPP
#define AUTONPATHPARSER_HPP

#include "../commands/odomMovement.hpp"
#include "autonVersioningUnion.hpp"
#include "dataPointType.hpp"

#include <vector>
#include <iostream>

#define PARSER_VER_MAJOR_COMPAT 0
#define PARSER_VER_MINOR_COMPAT 0
#define PARSER_VER_PATCH_COMPAT 0

class autonPathParser {
private:
    std::vector<char> autonPathData;
    VersioningUnion dataVer;
    OdomMovement* odomMovement;

    void checkVersion() {
        VersioningUnion parserCompatVer;
            parserCompatVer.version.versionMajor = PARSER_VER_MAJOR_COMPAT;
            parserCompatVer.version.versionMinor = PARSER_VER_MINOR_COMPAT;
            parserCompatVer.version.versionPatch = PARSER_VER_PATCH_COMPAT;

        if ((parserCompatVer.versionNum - this->dataVer.versionNum) < 0) {
            std::cout << "WARNING: PATH PARSER VERSION IS NOT COMPATIBLE WITH DATA VERSION. UPDATE PATH PARSER PLZ AND THANK YOU!" << std::endl;
        }
    }

    void init() {
        int currentOffset = 0;

        // read version num & output warning if not compatible
        this->dataVer.versionNum = *((uint64_t*)((void*)autonPathData.data()));
            currentOffset += sizeof(uint64_t);
            checkVersion();

        while(currentOffset < autonPathData.size()) {
            // gets next data point type
            switch(*((DataPointType*)(autonPathData.data()+currentOffset))) {
                
            }
        }
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