//
// Created by Matt on 3/2/2024.
//

#pragma once


#include "Image.h"

#include <vector>
#include <cmath>
#include <cassert>

namespace pdcpp
{

    using Matrix = std::vector<std::vector<float>>;

    // compute sigma^2 that 'fit' the kernel half width
    float compute_squared_variance(int half_width, float epsilon = 0.001)
    {
        assert(0 < epsilon && epsilon < 1); // small value required
        return - (half_width + 1.0) * (half_width + 1.0) / 2.0 / std::log(epsilon);
    }

    float gaussian_exp(float y, float x, float sigma2)
    {
        assert(0 < sigma2);
        return std::exp( - (x*x + y*y) / (2 * sigma2) );
    }

    // create a Gaussian kernel of size 2*half_width+1 x 2*half_width+1
    Matrix make_gaussian_kernel(int half_width)
    {
        if(half_width <= 0)
        {
            // kernel of size 1 x 1
            Matrix kernel(1, std::vector<float>(1, 1.0));
            return kernel;
        }

        Matrix kernel(2*half_width+1, std::vector<float>(2*half_width+1, 0.0));

        const float sigma2 = compute_squared_variance(half_width, 0.1);

        float sum = 0;
        for(int di = -half_width; di <= +half_width; ++di)
        {
            const int i = half_width + di;
            for(int dj = -half_width; dj <= +half_width; ++dj)
            {
                const int j = half_width + dj;
                kernel[i][j] = gaussian_exp(di, dj, sigma2);
                sum += kernel[i][j];
            }
        }

        assert(0 < sum);

        // normalize
        for(int i=0; i<2*half_width+1; ++i)
        {
            for(int j=0; j<2*half_width+1; ++j)
            {
                kernel[i][j] /= sum;
            }
        }

        return kernel;
    }

    pdcpp::Image gaussianBlur(const pdcpp::Image& img, int pxRadius);
}
