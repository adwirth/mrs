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
    int openFile(const string& fileName, ifstream& f);
    int readFileToBuffer(const string& fileName, char*& buffer);
};

#endif




