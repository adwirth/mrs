#include "fileIO.h"
#include "dlog.h"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>


int FileIO::openFile(const string& fileName, ifstream& f) 
{

    /*
        inline bool exists_test3 (const std::string& name) {
        struct stat buffer;   
        return (stat (name.c_str(), &buffer) == 0); 
        }
    */
    f = ifstream(fileName.c_str());
    if (!f.good())
    {
        return -1;
    }
    f.open(fileName, ios::binary);
    return 0;
}

int FileIO::readFileToBuffer(const string& fileName, char*& buffer) 
{
    std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
    size_t fileSize = in.tellg(); 
    buffer = new char[fileSize];

    DLOG << "File size: " << fileSize << endl;
    
    ifstream f;
//    openFile(fileName, f);
    if (!f.good())
    {
        return -1;
    }
    f.open(fileName, ios::binary);
    f.read(buffer, fileSize);    
    f.close();    

    DLOG << "buffer: " <<  *reinterpret_cast<const int8_t*>(buffer+3) << " " <<  *reinterpret_cast<const int8_t*>(buffer+1) << endl;
    // try 
    // {
        
    // } catch(fs::filesystem_error& e) 
    // {
    //     std::cout << e.what() << endl;
    // }   
    return 0;
}