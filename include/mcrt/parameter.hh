#ifndef MCRT_PARAMETER_HH
#define MCRT_PARAMETER_HH

namespace mcrt {
    struct Parameters {
        enum class ParallelFramework {
            NONE, OPENMP,
            OPENMPI, CILK
        };

        enum class SamplingMethod {
            GRID, ROTATE_GRID, RAND
        };

        ParallelFramework parallelFramework { ParallelFramework::NONE };
        size_t resolutionWidth { 256 }, resolutionHeight { 256 };
        double scalingFactorX  { 1.0 }, scalingFactorY   { 1.0 };
        Image::ResizeMethod interpolationMethod { Image::ResizeMethod::BILINEAR };
        SamplingMethod samplingMethod { SamplingMethod::ROTATE_GRID };
        size_t supersamplesPerPixel { 1 };
    };
}

#endif
