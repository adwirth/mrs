#ifndef FFTSCAN_H
#define FFTSCAN_H

#include <fftw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdint>
#include <algorithm>

using namespace std;

class FFTScan
{
    public:

    void calcMagnitude(float const* fd, float*& magn, size_t num);    
    void calcPhase(float const* fd, float*& phase, size_t num);
    void performFFT(float const* fd, float*& recon, int32_t dim1, int32_t dim2);

    private:

    void fftShift(fftwf_complex *dataIn, fftwf_complex *dataOut, int32_t dim1, int32_t dim2 );
};


#endif