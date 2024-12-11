#ifndef AUTONVERSIONINGUNION_HPP
#define AUTONVERSIONINGUNION_HPP

#include <cstdint>

typedef union {
    struct {
        uint32_t versionMajor;
        uint16_t versionMinor;
        uint16_t versionPatch;
    } version;
    uint64_t versionNum;
} VersioningUnion;

#endif