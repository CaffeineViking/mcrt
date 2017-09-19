#include "mcrt/param_import.hh"

#include <fstream>
#include <stdexcept>
#include "json.hh"

mcrt::Parameters mcrt::ParameterImporter::load(const std::string& file) {
    std::ifstream fileStream { file };
    nlohmann::json parser;
    fileStream >> parser;

    Parameters parameters;

    if (parser.find("parallelFramework") != parser.end()) {
        std::string parallel { parser["parallelFramework"].get<std::string>() };
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
        parameters.supersamplesPerPixel = supersampleAmount;

        std::string sampleMethod { supersamples[0][1].get<std::string>() };
        if (sampleMethod == "grid") parameters.samplingMethod = Parameters::SamplingMethod::GRID;
        else if (sampleMethod == "rotated") parameters.samplingMethod = Parameters::SamplingMethod::ROTATE_GRID;
        else if (sampleMethod == "rand") parameters.samplingMethod = Parameters::SamplingMethod::RAND;
        else std::runtime_error { "Error: no support for the '" + sampleMethod + "' pattern!" };
    }

    return parameters;
}
