#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

#include "mcrt/param_import.hh"
#include "mcrt/scene_import.hh"
#include "mcrt/parameter.hh"
#include "mcrt/scene.hh"

#include "mcrt/image.hh"
#include "mcrt/supersample.hh"
#include "mcrt/image_export.hh"

int usage(int argc, char** argv) {
    if (argc < 2) std::cerr << "Error: need the path to render scenes to!" << std::endl;
    std::cerr << "Usage: " << argv[0] << " "
              << "IMAGE [SCENE] [PARAMETER]"
              << std::endl;
    return 1;
}

void printProgress(const std::string& task, double progress, size_t characters = 70) {
    std::cout << task << "[";
    size_t position = progress * characters;
    for (size_t i { 0 }; i < characters; ++i) {
        if (i < position) std::cout << "=";
        else if (i > position) std::cout << " ";
        else std::cout << ">";
    } std::cout << "] ";

    size_t percent = progress * 100.0;
    std::cout << percent << " %\r";
    std::cout.flush();
}

int main(int argc, char** argv) {
    if (argc == 1) return usage(argc, argv);
    if (argc == 2 && std::strcmp(argv[1], "-h") == 0) {
        // User seems to be requesting for help...
        return usage(argc, argv);
    }

    std::string renderImagePath;
    mcrt::Parameters parameters;
    mcrt::Scene scene;

    if (argc > 1) renderImagePath = argv[1];
    else return usage(argc, argv); // Too few arguments.
    if (argc > 2) scene = mcrt::SceneImporter::load(argv[2]);
    if (argc > 3) parameters = mcrt::ParameterImporter::load(argv[3]);
    if (argc > 4) return usage(argc, argv); // Now it's just too many.

    // Shorthands for enabling or disabling the parallel framework under run-time. TODO: OpenMPI.
    bool openmp  { parameters.parallelFramework == mcrt::Parameters::ParallelFramework::OPENMP };
    const mcrt::Supersampler sampler { parameters.samplesPerPixel,  parameters.samplingPattern };

    // Note that render background will be transparent.
    mcrt::Image renderImage { parameters.resolutionWidth,
                              parameters.resolutionHeight };
    const mcrt::Camera& sceneCamera { scene.getCamera() };

    // Combine settings from the scenes and trace parameters.
    double fieldOfView { scene.getCamera().getFieldOfView() };
    scene.getCamera().setAspectRatio(renderImage.getAspectRatio());
    scene.getCamera().setFieldOfView(fieldOfView);

    auto renderStart  { std::chrono::steady_clock::now() };

    // ====================================================

    size_t rowsRendered { 0 }; // Shared resource.
    const double rowCount = renderImage.getHeight();
    double samplesPerPixel = parameters.samplesPerPixel;
    const glm::dvec3 eyePoint { sceneCamera.getEyePosition() };

    #pragma omp parallel for schedule(dynamic) if (openmp)
    for (size_t y = 0; y < renderImage.getHeight(); ++y) {
        double renderProgress = rowsRendered / rowCount;

        #pragma omp critical
        printProgress("Ray tracing: ",
                      renderProgress);
        #pragma omp atomic
        ++rowsRendered;

        // ------------------------------------------------

        for (size_t x = 0; x < renderImage.getWidth(); ++x) {
            std::vector<glm::dvec3> pixelColors;
            pixelColors.reserve(parameters.samplesPerPixel);
            // Below is the interval in the pixel where we can get further pp samples.
            auto samplingPlane = sceneCamera.getPixelSamplingPlane(renderImage, x, y);
            for (size_t i = 0; i < parameters.samplesPerPixel; ++i) {
                // Choose some location for us to sample in the pixel plane.
                glm::dvec3 viewPlanePoint { sampler.next(samplingPlane, i) };
                glm::dvec3 rayDirection { glm::normalize(viewPlanePoint - eyePoint) };
                mcrt::Ray rayFromViewPlane { viewPlanePoint, rayDirection };
                pixelColors.push_back(scene.rayTrace(rayFromViewPlane, 0));
            }

            // Here we sum contributions from each sample and then average the color's values.
            glm::dvec3 pixelColorSum = std::accumulate(pixelColors.begin(), pixelColors.end(),
                                                       glm::dvec3 { 0.0, 0.0, 0.0 });
            glm::dvec3 meanPixelColor { pixelColorSum / samplesPerPixel };
            renderImage.pixel(x, y) = meanPixelColor;
        }

        // ------------------------------------------------
    }

    // ====================================================

    auto renderFinish { std::chrono::steady_clock::now() };

    printProgress("Ray tracing: ", 1.0); // Might not be 100% in output.
    std::cout << std::endl; // Reset buffer after progress bar flush() hack.
    std::chrono::duration<double> renderDuration { renderFinish - renderStart };
    size_t renderTimeInSeconds = renderDuration.count();
    size_t renderTimeInMinutes = renderTimeInSeconds / 60.0;
    renderTimeInSeconds = renderTimeInSeconds % 60;
    std::cout << "Render took: " << renderTimeInMinutes << " minutes and "
                                 << renderTimeInSeconds << " seconds."
                                 << std::endl;

    size_t scaledWidth  = parameters.resolutionWidth  * parameters.scalingFactorX,
           scaledHeight = parameters.resolutionHeight * parameters.scalingFactorY;
    renderImage.resize(scaledWidth, scaledHeight, parameters.interpolationMethod);
    mcrt::ImageExporter::save(renderImage, renderImagePath);
    std::cout << "Rendered to: '" << renderImagePath << "'." << std::endl;
    return 0;
}
