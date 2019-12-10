#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>

using namespace std;
namespace fs = experimental::filesystem;

/**
 * Class for performing file IO operations
 */
class FileIO
{
    public:

    /**
     * Constructor
     */
    FileIO(const string& aFileName);

    /**
     * Performs file read to buffer
     */
    int readFileToBuffer(char*& aBuffer);

    /**
     * Opens an input file for reading
     */
    int openInputFile();

    /**
     * OPens an output file for writing
     */
    int openOutputFile();

    /**
     * Writes out buffer to disk.
     */
    int rawDump(const char* aBuffer, size_t aBufferSize);
    
    private:

    /**
     * Calculates file siye
     */
    int getFileSize();
    
    size_t          m_fileSize;      //!< Size of file
    const string&   m_fileName;      //!< Filename
    std::ifstream   m_fstream;       //!< Input file stream
    std::ofstream   m_outstream;     //!< Output file stream
};

#endif




