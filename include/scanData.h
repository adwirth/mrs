#ifndef SCANDATA_H
#define SCANDATA_H

#include <iostream>

using namespace std;


/**
 * ScanData
 * 
 * This class manages the read raw data and performs mappings.
 */
class ScanData
{
    public:

    /**
     * Enum for representing the different possible types of scan data.
     */
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

    int numTypeSizes[7]{1,1,2,2,4,4,8};         //!< Sizes of the datatypes

    /**
     * Template struct for representing 2D spectrometer data of various types
     */
    template <typename T> struct SpData2D
    {
        T const* d;
        size_t bytes;
        size_t num;
        NumType t;
        bool complex;
        int32_t dim1;
        int32_t dim2;
    };

    /**
     * Process file buffer and maps to FileMap
     */
    int procBuffer(const char* const buffer);

    SpData2D<float> m_spdf;                             //!< 2D spectrometer data

    private:
    
    /**
     * Packed struct for mapping file header
     */
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

    /**
     * Struct for mapping the whole file content
     */
    struct FileMap
    {
        const Header* h;
        const char* textData;
        const char* spData;
        const char* sampleFileName;
        const char* parameters;
    };

    /**
     * Fills spectroscopy data
     */
    int fillSpData2D();    
    FileMap m_fm;                                   //!< File map struct
};

#endif