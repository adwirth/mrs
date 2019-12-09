#include "fileIO.h"
#include "dlog.h"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>

FileIO::FileIO(const string& aFileName):
    m_fileName(aFileName)
{
       
}

int FileIO::openInputFile() 
{
    getFileSize();

    m_fstream = ifstream();
    m_fstream.open(m_fileName, ifstream::binary);
    DLOG << m_fileName <<endl;
    return 0;
}

int FileIO::getFileSize()
{
    std::ifstream f(m_fileName, std::ifstream::ate | std::ifstream::binary);
    m_fileSize = static_cast<size_t>(f.tellg());
    f.close();
    return 0;
}

int FileIO::readFileToBuffer(char*& aBuffer) 
{
    DLOG <<  "m_fileSize: " << (size_t)m_fileSize << endl;
    aBuffer = new char[m_fileSize];

    m_fstream.read(aBuffer, m_fileSize);
    if (!m_fstream)
    {
        std::cerr << "Error: only " << m_fstream.gcount() << " could be read!" << endl;
        m_fstream.close();
        delete[] aBuffer;
        return -1;
    }

    m_fstream.close();

    return 0;
}

int FileIO::openOutputFile() 
{
    m_outstream = ofstream();
    m_outstream.open(m_fileName, ofstream::binary);

    return 0;
}


int FileIO::rawDump(const char* aBuffer, size_t aBufferSize)
{
    m_outstream.write(aBuffer, aBufferSize);
    m_outstream.close();
    return 0;
}
