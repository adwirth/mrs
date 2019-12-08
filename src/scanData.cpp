#include "scanData.h"
#include "fileIO.h"
#include "dlog.h"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>

int ScanData::procBuffer(const char* const buffer)
{
    FileMap fm;
    size_t p = 0;
    fm.h = reinterpret_cast<const Header*>(buffer);

    p += 256;
    
    fm.textData = buffer + p;
    p += 256; // Unused text data omitted.

    DLOG << "d1: " << fm.h->dim1 << endl;
    DLOG << "d2: " << fm.h->dim2 << endl;
    DLOG << "d3: " << fm.h->dim3 << endl;
    DLOG << "d4: " << fm.h->dim4 << endl;
    DLOG << "d5: " << fm.h->dim5 << endl;
    DLOG << "dtype: " << fm.h->dtype << endl;


    bool complex = fm.h->dtype & 0x10;
    int numtype = fm.h->dtype & 0x0f;

    SpData d;
    d.complex = complex;
    d.t = NumType(numtype);
    d.num = fm.h->dim1*fm.h->dim2*fm.h->dim3*fm.h->dim4*fm.h->dim5;
    d.bytes = d.num * numTypeSizes[numtype] * (d.complex ? 2 : 1);
    d.d = reinterpret_cast<const float*>(buffer + p);
    
    p += d.bytes;

    DLOG << "complex: " << d.complex << " numtype: " << d.t << " num: " << d.num << " bytes " << d.bytes << endl;

    size_t sampleFileNameLength = 120;
    fm.sampleFileName = buffer + p;
    DLOG << fm.sampleFileName << endl;

    p += sampleFileNameLength;

    fm.parameters = buffer + p;

    DLOG << "parameters: " << fm.parameters << endl;

    if (!complex || numtype != 5)
    {
        cerr << "Currently only complex float datatype is supported!" << endl;
        exit(0);
    }

    return 0;
}