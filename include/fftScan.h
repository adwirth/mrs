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

    static void calcMagnitude(float const* fd, size_t num)
    {
        float* magn = new float[num];
        for (size_t i = 0; i < num; ++i)
        {
            magn[i] = fd[2 * i] * fd[2 * i] + fd[2 * i + 1] * fd[2 * i + 1];
        }
        ofstream f;
        f.open("magn.raw", ios::binary);
        f.write(reinterpret_cast<char*>(magn), sizeof(float) * num);
        f.close();
    }

    static void fftShift(fftwf_complex *dataIn, fftwf_complex *dataOut, int32_t dim1, int32_t dim2 )
    {
        for (int i = 0; i < dim1; ++i)
        {
            for (int j = 0; j < dim2; ++j)
            {
                int32_t iShift = (dim1/2 - i) % dim1;
                int32_t jShift = (dim2/2 - j) % dim2;

                if (iShift < 0)
                    iShift += dim1;
                if (jShift < 0)
                    jShift += dim2;
              
                dataOut[i + j*dim1][0] = dataIn[iShift + jShift*dim1][0];
                dataOut[i + j*dim1][1] = dataIn[iShift + jShift*dim1][1];
            }
        }
    }

    static void performFFT(float const* fd, int32_t dim1, int32_t dim2)
    {
        int N;
        fftwf_complex *in, *inShift, *outShift, *out;
        fftwf_plan my_plan;
        in = (fftwf_complex*) fd;        
        out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex)*dim1*dim2);
        my_plan = fftwf_plan_dft_2d(dim2, dim1, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftwf_execute(my_plan);
        fftwf_destroy_plan(my_plan);
        outShift = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*dim1*dim2);        
        fftShift(out, outShift, dim1, dim2);
        calcMagnitude(reinterpret_cast<float const*>(outShift), dim1*dim2);
        fftwf_free(out);
    }
};


#endif