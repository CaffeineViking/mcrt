#include "mcrt/parameter.hh"

#include <fstream>
#include <iostream>

void mcrt::Parameters::writeStatistics(const std::string& renderPath, double seconds) {
    bool writeHeader = false;
    std::ifstream checkFile { "statistics.csv" };
    if (!checkFile.good()) writeHeader = true;
    checkFile.close();

    std::ofstream fileStream { "share/statistics.csv", std::fstream::app };
    if (writeHeader) fileStream << "parallelFramework,resolutionWidth,resolutionHeight,scalingFactorX,scalingFactorY,"
                                << "interpolationMethod,samplingPattern,samplesPerPixel,maxRayDepth,shadowRayCount,"
                                << "photonEstimationRadius,photonAmount,photonMap,progressiveRendering,renderPath,renderTime"
                                << std::endl;

    fileStream << *this;
    fileStream << renderPath << ',';
    fileStream << seconds << std::endl;
    std::cout << "Statistics in: '" << "share/statistics.csv'." << std::endl;
}

std::ostream& operator<<(std::ostream& output, const mcrt::Parameters& parameters) {
    if (parameters.parallelFramework == mcrt::Parameters::ParallelFramework::NONE) output << "none" << ',';
    else if (parameters.parallelFramework == mcrt::Parameters::ParallelFramework::OPENMP) output << "openmp" << ',';
    else if (parameters.parallelFramework == mcrt::Parameters::ParallelFramework::OPENMPI) output << "openmpi" << ',';

    output << parameters.resolutionWidth << ',' << parameters.resolutionHeight << ',';
    output << parameters.scalingFactorX << ',' << parameters.scalingFactorY << ',';

    if (parameters.interpolationMethod ==  mcrt::Image::ResizeMethod::BILINEAR) output << "bilinear" << ',';
    else if (parameters.interpolationMethod ==  mcrt::Image::ResizeMethod::NEAREST_NEIGHBOR) output << "nearest" << ',';

    if (parameters.samplingPattern == mcrt::Supersampler::Pattern::GRID) output << "grid" << ',';
    else if (parameters.samplingPattern == mcrt::Supersampler::Pattern::RANDOM) output << "rand" << ',';
    else if (parameters.samplingPattern == mcrt::Supersampler::Pattern::GAUSSIAN) output << "norm" << ',';

    output << parameters.samplesPerPixel << ',' << parameters.maxRayDepth << ',' << parameters.shadowRayCount << ',';
    output << parameters.photonEstimationRadius << ',' << parameters.photonAmount << ',';
    output << parameters.photonMap << ',' << parameters.progressiveRendering << ',';
    return output;
}
