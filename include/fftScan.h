#ifndef FFTSCAN_H
#define FFTSCAN_H

#include <fftw3.h>
#include <algorithm>

using namespace std;

/**
 * FFT and complex array processing class
 */
class FFTScan
{
    public:

    /**
     * Calculate magnitude array from complex array
     */
    void calcMagnitude(float const* fd, float*& magn, size_t num);

    /**
     * Calculate phase array from complex array
     */
    void calcPhase(float const* fd, float*& phase, size_t num);

    /**
     * Performs FFT
     */
    void performFFT(float const* fd, float*& recon, int32_t dim1, int32_t dim2);

    private:

    /**
     * Performs shift on complex data
     */
    void fftShift(fftwf_complex *dataIn, fftwf_complex *dataOut, int32_t dim1, int32_t dim2 );
};

#endif