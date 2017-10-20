#include "mcrt/param_import.hh"

#include <fstream>
#include <stdexcept>
#include "json.hh"

mcrt::Parameters mcrt::ParameterImporter::load(const std::string& file) {
    std::ifstream fileStream { file };
    nlohmann::json parser;
    fileStream >> parser;

    Parameters parameters;

    if (parser.find("progressiveRendering") != parser.end()) {
        size_t progressive { parser["progressiveRendering"].get<size_t>() };
        if (progressive > 0) parameters.progressiveRendering = true;
        else parameters.progressiveRendering = false;
    }

    if (parser.find("parallelMethod") != parser.end()) {
        std::string parallel { parser["parallelMethod"].get<std::string>() };
        if (parallel == "none") parameters.parallelFramework = Parameters::ParallelFramework::NONE;
        else if (parallel == "openmp") parameters.parallelFramework = Parameters::ParallelFramework::OPENMP;
        else if (parallel == "openmpi") parameters.parallelFramework = Parameters::ParallelFramework::OPENMPI;
        else std::runtime_error { "Error: no support for parallel framework '" + parallel + "'!" };
    }

    if (parser.find("resolution") != parser.end()) {
        nlohmann::json resolution { parser["resolution"] };
        if (resolution.size() != 2) std::runtime_error { "Error: resolution parameter is malformed!" };
        size_t resolutionWidth  { resolution[0][0].get<size_t>() };
        size_t resolutionHeight { resolution[0][1].get<size_t>() };
        parameters.resolutionHeight = resolutionHeight;
        parameters.resolutionWidth  = resolutionWidth;
    }

    if (parser.find("scalingFactor") != parser.end()) {
        nlohmann::json scalingFactor { parser["scalingFactor"] };
        if (scalingFactor.size() != 2) std::runtime_error { "Error: scaling factor seems malformed!" };
        double scalingFactorX { scalingFactor[0][0].get<double>() };
        double scalingFactorY { scalingFactor[0][1].get<double>() };
        parameters.scalingFactorX = scalingFactorX;
        parameters.scalingFactorY = scalingFactorY;
    }

    if (parser.find("interpolation") != parser.end()) {
        std::string interpolation { parser["interpolation"].get<std::string>() };
        if (interpolation == "bilinear")
            parameters.interpolationMethod = Image::ResizeMethod::BILINEAR;
        else if (interpolation == "nearest")
            parameters.interpolationMethod = Image::ResizeMethod::NEAREST_NEIGHBOR;
        else std::runtime_error { "Error: no support for '" + interpolation + "' interpolation!" };
    }

    if (parser.find("supersamples") != parser.end()) {
        nlohmann::json supersamples { parser["supersamples"] };
        if (supersamples.size() != 2) std::runtime_error { "Error: supersamples parameters are malformed!" };
        size_t supersampleAmount { supersamples[0][0].get<size_t>() };
        // We square this since we'll set 4x4 = 16 implicitly with sampler.
        parameters.samplesPerPixel = supersampleAmount * supersampleAmount;

        std::string sampleMethod { supersamples[0][1].get<std::string>() };
        if (sampleMethod == "grid") parameters.samplingPattern = Supersampler::Pattern::GRID;
        else if (sampleMethod == "rand") parameters.samplingPattern = Supersampler::Pattern::RANDOM;
        else if (sampleMethod == "norm") parameters.samplingPattern = Supersampler::Pattern::GAUSSIAN;
        else std::runtime_error { "Error: no support for this: '" + sampleMethod + "' pattern!" };
    }

    if (parser.find("maxRayDepth") != parser.end()) {
        size_t maxRayDepth { parser["maxRayDepth"].get<size_t>() };
        parameters.maxRayDepth = maxRayDepth;
    }

    if (parser.find("shadowRays") != parser.end()) {
        parameters.shadowRayCount = parser["shadowRays"].get<size_t>();
    }

    if (parser.find("photonAmount") != parser.end()) {
        parameters.photonAmount = parser["photonAmount"].get<size_t>();
    }

    return parameters;
}
