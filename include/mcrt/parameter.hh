#ifndef MCRT_PARAMETER_HH
#define MCRT_PARAMETER_HH

#include <iostream>

#include "mcrt/supersample.hh"

namespace mcrt {
    struct Parameters {
        enum class ParallelFramework {
            NONE, OPENMP, OPENMPI
        };

        ParallelFramework parallelFramework { ParallelFramework::NONE };
        size_t resolutionWidth { 256 }, resolutionHeight { 256 };
        double scalingFactorX  { 1.0 }, scalingFactorY   { 1.0 };
        Image::ResizeMethod interpolationMethod { Image::ResizeMethod::BILINEAR };
        Supersampler::Pattern samplingPattern { Supersampler::Pattern::GRID };
        size_t samplesPerPixel { 1 };
        size_t maxRayDepth { 10 };
        size_t shadowRayCount { 1 };
        double photonEstimationRadius { 1.0 };
        size_t photonAmount { 100000 };
        bool photonMap { true };
        bool progressiveRendering { false };
        bool recordStatistics { false };

        void writeStatistics(const std::string&, double);
    };

}

std::ostream& operator<<(std::ostream& output, const mcrt::Parameters&);

#endif
