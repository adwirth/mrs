#include "dlog.h"
#include "scanData.h"
#include "fileIO.h"
#include "fftScan.h"
#include "evalIQ.h"
#include <unistd.h>
#include <iostream>

/**
 * Find command line paramter.
 */
int findParamIndex(const char **argv, int argc, const char *parm) 
{
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
                  << endl;
        return -1;
    }
}

int main(int argc, const char *argv[])
{
    // Command line arguments parsing 

    int pidx;
    string scanMagnFilename;
    string scanPhaseFilename;
    string outputMagnFilename;
    string outputPhaseFilename;
    
    if ((pidx = findParamIndex(argv, argc, "-h")) != -1 ||
        (pidx = findParamIndex(argv, argc, "--help")) != -1 ||
         argc == 1) 
    {
        std::cout   << "Usage: " << argv[0]
                    << " <input MRD>" << endl
                    << " --scanmag <ScanMagnFileName>" << endl 
                    << " --scanphase <scanPhaseFilename>" << endl 
                    << " --outmag <outputMagnFilename>" << endl 
                    << " --outphase <outputPhaseFilename>" << endl;
        return EXIT_SUCCESS;
    }

    const string inputFileName(argv[1]);

    if ((pidx = findParamIndex(argv, argc, "--scanmag")) != -1)
    {
       scanMagnFilename = argv[pidx + 1];
    }

    if ((pidx = findParamIndex(argv, argc, "--scanphase")) != -1)
    {
       scanPhaseFilename = argv[pidx + 1];
    }

    if ((pidx = findParamIndex(argv, argc, "--outmag")) != -1)
    {
       outputMagnFilename = argv[pidx + 1];
    }

    if ((pidx = findParamIndex(argv, argc, "--outphase")) != -1)
    {
       outputPhaseFilename = argv[pidx + 1];
    }  
    std::cout << "  [Info] [" << getpid() << "] Parsing operations done." << endl;

    // Reading scan file and mapping data
    
    char* buffer;
    FileIO fio1(inputFileName);  
    fio1.openInputFile();
    fio1.readFileToBuffer(buffer);

    std::cout << "  [Info] [" << getpid() << "] File reading done." << endl;

    ScanData scand;
    scand.procBuffer(buffer);

    std::cout << "  [Info] [" << getpid() << "] Scan data processing done." << endl;

    FFTScan fft;

    // Calculate input scan magnitude

    if (scanMagnFilename != "")
    {
        float* magn = nullptr;
        fft.calcMagnitude(scand.m_spdf.d, magn, scand.m_spdf.num);
        FileIO fio(scanMagnFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(magn), scand.m_spdf.num * sizeof(float));
        delete[] magn;

        std::cout << "  [Info] [" << getpid() << "] Input scan magnitude calculated." << endl;
    }

    // Calculate input scan phase

    if (scanPhaseFilename != "")
    {
        float* phase = nullptr;
        fft.calcPhase(scand.m_spdf.d, phase, scand.m_spdf.num);
        FileIO fio(scanPhaseFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(phase), scand.m_spdf.num * sizeof(float));
        delete[] phase;

        std::cout << "  [Info] [" << getpid() << "] Input scan phase calculated." << endl;
    }

    // Perform FFT

    float* recon = nullptr;
    fft.performFFT(scand.m_spdf.d, recon, scand.m_spdf.dim1, scand.m_spdf.dim2);
    std::cout << "  [Info] [" << getpid() << "] FFT performed." << endl;

    // Calculate reconstruction magnitude

    if (outputMagnFilename != "")
    {
        float* magn = nullptr;
        fft.calcMagnitude(recon, magn, scand.m_spdf.num);
        FileIO fio(outputMagnFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(magn), scand.m_spdf.num * sizeof(float));
        cout << endl << "SNR: " << EvalIQ::calcSNR(magn, scand.m_spdf.dim1, scand.m_spdf.dim2, scand.m_spdf.dim1 / 2.f, scand.m_spdf.dim2 / 2.f, 10.f) << endl << endl;
        delete[] magn;

        std::cout << "  [Info] [" << getpid() << "] Reconstructed data magnitude calculated." << endl;
    }

    // Calculate reconstruction phase

    if (outputPhaseFilename != "")
    {
        float* phase = nullptr;
        fft.calcPhase(recon, phase, scand.m_spdf.num);
        FileIO fio(outputPhaseFilename);  
        fio.openOutputFile();
        fio.rawDump(reinterpret_cast<const char*>(phase), scand.m_spdf.num * sizeof(float));
        delete[] phase;

        std::cout << "  [Info] [" << getpid() << "] Reconstructed data phase calculated." << endl;
    }

    delete[] recon;
    delete[] buffer;

    std::cout << "  [Info] [" << getpid() << "] Program execution finished." << endl;
    return 0;
}
