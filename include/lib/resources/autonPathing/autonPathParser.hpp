#ifndef AUTONPATHPARSER_HPP
#define AUTONPATHPARSER_HPP

#include "../commands/odomMovement.hpp"
#include "autonVersioningUnion.hpp"
#include "pointArray.hpp"
#include "dataPointType.hpp"
#include "points/point.hpp"
    #include "points/pointInitial.hpp"
    #include "points/linearPoint.hpp"
    #include "points/endPoint.hpp"

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
    PointArray pathPoints;
    int currentPointNum = 0;

    bool isBlueSide;
    float angleOffset = 0.0f;
    Vector2F botPlacement;

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
            // the constructors auto-increment currentOffset (I know it's terrible & idc)
            switch(*((DataPointType*)(autonPathData.data()+currentOffset))) {
                
                // this is weird but needs to be done after the switch statement condition starts
                currentOffset += sizeof(DataPointType);
                
                case DataPointType_Settings: {
                    std::cout << "Settings are not implemented. Confirm path data is correct or update parser version." << std::endl;
                }
                break;

                case DataPointType_Initial: {
                    this->pathPoints.add(new PointInitial(DataPointType_Initial, autonPathData, currentOffset));
                }
                break;

                case DataPointType_LinearPoint: {
                    this->pathPoints.add(new LinearPoint(DataPointType_LinearPoint, autonPathData, currentOffset));
                }
                break;

                case DataPointType_Event: {
                    std::cout << "Event not implemented. Confirm path data is correct or update parser version." << std::endl;
                }
                break;

                case DataPointType_EndPoint: {
                    this->pathPoints.add(new EndPoint(DataPointType_EndPoint, autonPathData, currentOffset));
                }
                break;
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
        init();
    }

    void update() {
        Point* curr = pathPoints.at(this->currentPointNum);

        switch (curr->getPointType()) {
            case DataPointType_Initial: {
                PointInitial* currFixed = (PointInitial*)curr;

                this->isBlueSide = currFixed->isBlue;
                this->angleOffset = currFixed->angle;
                this->botPlacement = currFixed->getPointPlacement();
            }
            break;

            case DataPointType_LinearPoint: {
                LinearPoint* currFixed = (LinearPoint*)curr;

                
            }
            break;

            case DataPointType_EndPoint: {
                EndPoint* currFixed = (EndPoint*)curr;
            }
            break;
        }
    }
};



#endif