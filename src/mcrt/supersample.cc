#include "mcrt/supersample.hh"

#include <cmath>
#include <stdexcept>

glm::dvec3 mcrt::Supersampler::next(Camera::SamplingPlane& samplingPlane, size_t currentSample) const {
    if (samplingAmount == 1) return (samplingPlane.corners[0] + samplingPlane.corners[2]) / 2.0;
    switch (pattern) {
    case Pattern::GRID:   return grid(samplingPlane, currentSample);
    case Pattern::RANDOM: return prng(samplingPlane, currentSample);
    default: throw std::runtime_error { "Error: a unknown pattern!" };
    }
}

glm::dvec3 mcrt::Supersampler::grid(Camera::SamplingPlane& samplingPlane, size_t currentSample) const {
    glm::dvec3 xViewPlaneAxis { samplingPlane.corners[1] - samplingPlane.corners[0] },
               yViewPlaneAxis { samplingPlane.corners[3] - samplingPlane.corners[0] };
    double ySampleAxisWeight = std::floor(currentSample / samplingWidth),
           xSampleAxisWeight = currentSample - ySampleAxisWeight*samplingWidth;
    ySampleAxisWeight /= samplingWidth; xSampleAxisWeight /= samplingWidth;
    return samplingPlane.corners[0] + xViewPlaneAxis*xSampleAxisWeight
                                    + yViewPlaneAxis*ySampleAxisWeight;
}

glm::dvec3 mcrt::Supersampler::prng(Camera::SamplingPlane& samplingPlane, size_t) const {
    glm::dvec3 xViewPlaneAxis { samplingPlane.corners[1] - samplingPlane.corners[0] },
               yViewPlaneAxis { samplingPlane.corners[3] - samplingPlane.corners[0] };
    double xSampleAxisWeight { distribution(randomNumberGenerator) },
           ySampleAxisWeight { distribution(randomNumberGenerator) };
    return samplingPlane.corners[0] + xViewPlaneAxis*xSampleAxisWeight
                                    + yViewPlaneAxis*ySampleAxisWeight;
}
