#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>

using namespace std;
namespace fs = experimental::filesystem;

class FileIO
{
    public:

    FileIO(const string& aFileName);
    int readFileToBuffer(char*& aBuffer);
    int openInputFile();
    int openOutputFile();
    int rawDump(const char* aBuffer, size_t aBufferSize);
    
    private:

    int getFileSize();
    
    size_t          m_fileSize;
    const string&   m_fileName;
    std::ifstream   m_fstream;
    std::ofstream   m_outstream;
    
};

#endif




