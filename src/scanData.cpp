#include "scanData.h"
#include "fileIO.h"
#include "dlog.h"

#include <iostream>

int ScanData::fillSpData2D()
{
    m_spdf.complex = m_fm.h->dtype & 0x10;
    m_spdf.t = NumType(m_fm.h->dtype & 0x0f);
    m_spdf.num = m_fm.h->dim1*m_fm.h->dim2*m_fm.h->dim3*m_fm.h->dim4*m_fm.h->dim5;
    m_spdf.bytes = m_spdf.num * numTypeSizes[m_spdf.t] * (m_spdf.complex ? 2 : 1);
    m_spdf.d = reinterpret_cast<decltype(m_spdf.d)>(m_fm.spData);
    m_spdf.dim1 = m_fm.h->dim1;
    m_spdf.dim2 = m_fm.h->dim2;
    if (!m_spdf.complex || m_spdf.t != 5)
    {
        cerr << "Currently only complex float datatype is supported!" << endl;
        return -1;
    }
    if (m_fm.h->dim3 > 1 || m_fm.h->dim4 > 4 || m_fm.h->dim5 > 5)
    {
        cerr << "Currently only 2D arrays are supported!" << endl;
        return -1;
    }
    return 0;
}

int ScanData::procBuffer(const char* const aBuffer)
{    
    size_t p = 0;
    m_fm.h = reinterpret_cast<const Header*>(aBuffer);

    p += 256;
    
    m_fm.textData = aBuffer + p;

    p += 256; 

    DLOG << "d1: " << m_fm.h->dim1 << endl;
    DLOG << "d2: " << m_fm.h->dim2 << endl;
    DLOG << "d3: " << m_fm.h->dim3 << endl;
    DLOG << "d4: " << m_fm.h->dim4 << endl;
    DLOG << "d5: " << m_fm.h->dim5 << endl;
    DLOG << "dtype: " << m_fm.h->dtype << endl;

    m_fm.spData = aBuffer + p;
    
    fillSpData2D();
    
    p += m_spdf.bytes;

    DLOG << "complex: " << m_spdf.complex << " numtype: " << m_spdf.t << " num: " << m_spdf.num << " bytes " << m_spdf.bytes << endl;

    size_t sampleFileNameLength = 120;
    m_fm.sampleFileName = aBuffer + p;
    DLOG << m_fm.sampleFileName << endl;

    p += sampleFileNameLength;

    m_fm.parameters = aBuffer + p;

    return 0;
}