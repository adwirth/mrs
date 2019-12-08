#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>


struct X {
  ~X() { std::cout << std::endl; }
};
#ifndef NDEBUG 
#define debug_disabled 0
#else 
#define debug_disabled 1
#endif

#define DLOG \
    if (debug_disabled) {} \
    else std::cerr

using namespace std;
namespace fs = experimental::filesystem;

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

int numTypeSizes[]{1,1,2,2,4,4,8};

struct SpData
{
    const float* d;
    size_t bytes;
    size_t num;
    NumType t;
    bool complex;
};

// parse parameters
int findParamIndex(const char **argv, int argc, const char *parm) {
    int count = 0;
    int index = -1;

    for (int i = 0; i < argc; i++) {
        if (strncmp(argv[i], parm, 100) == 0) {
            index = i;
            count++;
        }
    }

    if (count == 0 || count == 1) {
        return index;
    } else {
        std::cout << "Error, parameter " << parm
                  << " has been specified more than once, exiting\n"
                  << std::endl;
        return -1;
    }
}

/*
std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}
*/


int openFile(const string& fileName, ifstream& f) 
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

int readFileToBuffer(const string& fileName, char*& buffer) 
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

int procBuffer(const char* const buffer)
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

int main(int argc, const char *argv[])
{
    int pidx;

    cout << "Hello MR!" << endl;

    if ((pidx = findParamIndex(argv, argc, "-h")) != -1 ||
        (pidx = findParamIndex(argv, argc, "--help")) != -1 ||
         argc == 1) 
    {
        std::cout   << "Usage: " << argv[0]
                    << " <input MRD>\n";
        return EXIT_SUCCESS;
    }

    const string inputFileName(argv[1]);
    
    char* buffer;
    readFileToBuffer(inputFileName, buffer);
    procBuffer(buffer);
    DLOG << buffer;

    return 0;
}
