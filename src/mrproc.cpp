#include "dlog.h"
#include "scanData.h"
#include "fileIO.h"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <cstring>
#include <cstdint>



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
    FileIO fio(inputFileName);  
    fio.openFile();
    fio.readFileToBuffer(buffer);

    ScanData scand;
    scand.procBuffer(buffer);

    delete[] buffer;
    return 0;
}
