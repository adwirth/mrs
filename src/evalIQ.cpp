#include "evalIQ.h"
#include <math.h>
#include <iostream>

float EvalIQ::calcSNR(const float* image, int32_t dim1, int32_t dim2, float center1, float center2, float radius)
{
    double std = 0.;
    double mean = 0.;
    double r2 = radius * radius;
    int n = 0;    

    // Using Welford's algorithm for simultaneous estimation of mean and variance

    for( int i = (int)(center1 - radius); i <= (int)(center1 + radius); ++i )
    {
        for( int j = (int)(center2 - radius); j <= (int)(center2 + radius); ++j )
        {
            if ( (i - center1) * (i - center1) + (j - center2) * (j - center2) < r2 )
            {
                ++n;
                float v = image[i + j * dim1];
                double delta = v - mean;
                mean += delta / n;
                std += delta * (v - mean );
            }
        }
    }

    return mean / (sqrt(std / (n - 1)));
}
