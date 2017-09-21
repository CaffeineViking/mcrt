#ifndef MCRT_SUPERSAMPLE_HH
#define MCRT_SUPERSAMPLE_HH

#include <random>
#include <glm/glm.hpp>
#include "mcrt/camera.hh"

namespace mcrt {
    class Supersampler {
    public:
        enum class Pattern {
            GRID, RANDOM,
            GAUSSIAN
        };

        Supersampler(size_t samplingAmount, Pattern pattern)
            : samplingAmount { samplingAmount },
              samplingWidth { std::sqrt(samplingAmount) },
              pattern { pattern },
              randomNumberGenerator { std::random_device {  }() } {  }

        Pattern getPattern() const { return this->pattern; }
        void setPattern(Pattern pattern) { this->pattern = pattern; }
        void setSamplingAmount(size_t amount) { samplingAmount = amount; }
        size_t getSamplingAmount() const { return samplingAmount; }
        size_t getSamplingWidth() const { return samplingWidth; }

        glm::dvec3 next(Camera::SamplingPlane&, size_t) const;

    private:
        // Below are the types of sample pattern distribution.
        glm::dvec3 grid(Camera::SamplingPlane&, size_t) const;
        glm::dvec3 prng(Camera::SamplingPlane&, size_t) const;
        glm::dvec3 norm(Camera::SamplingPlane&, size_t) const;

        size_t samplingAmount { 1 };
        double samplingWidth  { 1 };
        Pattern pattern  { Pattern::GRID };
        mutable std::mt19937 randomNumberGenerator;
        mutable std::normal_distribution<double> gaussian { 0.5, 0.5 };
        mutable std::uniform_real_distribution<double> uniform { 0.0, 1.0 };
    };
}

#endif
