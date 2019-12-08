#ifndef SCANDATA_H
#define SCANDATA_H

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>

using namespace std;

class ScanData
{
    public:
    #pragma pack(push, 1)
    struct Header
    {
        int32_t dim1;
        int32_t dim2;
        int32_t dim3;
        int32_t dim4;
        char    _unspec1[0x11-0xF];
        int16_t dtype;
        char    _unspec2[0x98-0x14];
        int32_t dim5;
        int32_t dim6;
    };
    #pragma pack(pop)

    struct FileMap
    {
        const Header* h;
        const char* textData;
        const char* spData;
        const char* sampleFileName;
        const char* parameters;
    };

    enum NumType
    {
        UCHAR,
        CHAR,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE
    };

    int numTypeSizes[7]{1,1,2,2,4,4,8};

    struct SpData
    {
        const float* d;
        size_t bytes;
        size_t num;
        NumType t;
        bool complex;
    };


    int procBuffer(const char* const buffer);

};

#endif