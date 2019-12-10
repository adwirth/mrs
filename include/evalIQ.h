#ifndef EVALIQ_H
#define EVALIQ_H

#include <algorithm>

using namespace std;

/**
 * Class for image quality evaluations
 */
class EvalIQ
{
    public:

    /**
     * Calculates SNR
     */
    static float calcSNR(const float* image, int32_t dim1, int32_t dim2, float center1, float center2, float radius);    
};


#endif