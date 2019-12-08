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

int FileIO::openFile() 
{
    /*
        inline bool exists_test3 (const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
        }
    */
    getFileSize();

    m_in = ifstream();
    m_in.open(m_fileName, ifstream::binary);
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

    m_in.read(aBuffer, m_fileSize);
    if (!m_in)
    {
        std::cerr << "Error: only " << m_in.gcount() << " could be read!" << endl;
        m_in.close();
        delete[] aBuffer;
        return -1;
    }

    m_in.close();

    return 0;
}