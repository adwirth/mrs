#include "dlog.h"
#include "scanData.h"
#include "fileIO.h"
#include "fftScan.h"

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

    string scanMagnFilename("inpmag.raw");
    string scanPhaseFilename("inpphase.raw");
    string outputMagnFilename("outmagn.raw");
    string outputPhaseFilename("outphase.raw");
    
    if ((pidx = findParamIndex(argv, argc, "-h")) != -1 ||
        (pidx = findParamIndex(argv, argc, "--help")) != -1 ||
         argc == 1) 
    {
        std::cout   << "Usage: " << argv[0]
                    << " <input MRD>\n";
        return EXIT_SUCCESS;
    }

        // std::cout   << "Usage: " << argv[0]
        //             << " <input jpeg> <output jpeg> [parameters]\n";
        // std::cout   << "Parameters: " << std::endl;
        // std::cout   << "\tq\t:\tJPEG quality of encoding"
        //             << std::endl;
        // std::cout   << "\ts\t:\tJPEG subsampling for encoding [444, 422, 420, 440, 411, 410, gray]"
        //             << std::endl;
        // std::cout   << "\tfmt\t:\tJPEG decoding color space [rgb, bgr, rgbi, bgri, yuv]"
        //             << std::endl;
        // std::cout   << "\tr\t:\trepeat the processing cicle N times (only the last one will write)"
        //             << std::endl;
        // std::cout   << "\tdevice_id\t:\tWhich device to use for decoding"
        //             << std::endl;
        // std::cout   << "\tbackend\t:\tWhich huffman decode backend should be used [default, hybrid, gpu]"
        //             << std::endl;
        // std::cout   << "\t3\t:\tUse 3 phase decoding"
        //             << std::endl;
        // std::cout   << std::endl;


        //     if (((pidx = findParamIndex(argv, argc, "-fmt")) != -1) || ((pidx = findParamIndex(argv, argc, "--fmt")) != -1) ) {
        // std::string sfmt = argv[pidx + 1];
        // if (sfmt == "rgb") {

    const string inputFileName(argv[1]);
    
    char* buffer;
    FileIO fio1(inputFileName);  
    fio1.openInputFile();
    fio1.readFileToBuffer(buffer);

    ScanData scand;
    scand.procBuffer(buffer);

    FFTScan fft;

    if (scanMagnFilename != "")
    {
        float* magn = nullptr;
        fft.calcMagnitude(scand.m_spdf.d, magn, scand.m_spdf.num);
        FileIO fio(scanMagnFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(magn), scand.m_spdf.num * sizeof(float));
        delete[] magn;
    }

    if (scanPhaseFilename != "")
    {
        float* phase = nullptr;
        fft.calcPhase(scand.m_spdf.d, phase, scand.m_spdf.num);
        FileIO fio(scanPhaseFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(phase), scand.m_spdf.num * sizeof(float));
        delete[] phase;
    }

    float* recon = nullptr;
    fft.performFFT(scand.m_spdf.d, recon, scand.m_spdf.dim1, scand.m_spdf.dim2);

    if (outputMagnFilename != "")
    {
        float* magn = nullptr;
        fft.calcMagnitude(recon, magn, scand.m_spdf.num);
        FileIO fio(outputMagnFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(magn), scand.m_spdf.num * sizeof(float));
        delete[] magn;
    }

    if (outputPhaseFilename != "")
    {
        float* phase = nullptr;
        fft.calcPhase(recon, phase, scand.m_spdf.num);
        FileIO fio(outputPhaseFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(phase), scand.m_spdf.num * sizeof(float));
        delete[] phase;
    }

    delete[] recon;
    delete[] buffer;
    return 0;
}
