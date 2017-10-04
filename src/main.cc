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

#include "mcrt/material.hh"

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

    mcrt::Scene::maxRayDepth = parameters.maxRayDepth;
    mcrt::AreaLight::shadowRayCount = parameters.shadowRayCount;

    auto renderStart  { std::chrono::steady_clock::now() };

    // ================== Ray Tracing Step ====================

    size_t pixelSamplesTaken { 0 }; // Shared resource.
    const size_t imagePixels { renderImage.getSize() };
    double samplesPerPixel = parameters.samplesPerPixel;
    double totalPixelSamples { samplesPerPixel * imagePixels };
    const glm::dvec3 eyePoint { sceneCamera.getEyePosition() };

    for (size_t i = 0; i < samplesPerPixel; ++i) {

        // -------------------- Ray Trace ---------------------

        #pragma omp parallel for schedule(dynamic) if (openmp)
        for (size_t y = 0; y < renderImage.getHeight(); ++y) {

            #pragma omp critical
            printProgress("Ray tracing: ", pixelSamplesTaken /
                                           totalPixelSamples);
            #pragma omp atomic
            pixelSamplesTaken += renderImage.getWidth();

            for (size_t x = 0; x < renderImage.getWidth(); ++x) {

                // Below is the interval in the pixel where we can get further pp samples.
                auto samplingPlane = sceneCamera.getPixelSamplingPlane(renderImage, x, y);

                // Here we actually fetch the next sampling position to take.
                glm::dvec3 viewPlanePoint { sampler.next(samplingPlane, i) };
                // Find our where in the scene our eye's pixel sample is looking at...
                glm::dvec3 rayDirection { glm::normalize(viewPlanePoint - eyePoint) };
                mcrt::Ray rayFromViewPlane { viewPlanePoint, rayDirection };

                // Finally, raytrace through the scene and get the pixel irradiance.
                glm::dvec3 colorPixelSample { scene.rayTrace(rayFromViewPlane, 0) };
                // Since this is just one sample, it should only contribute a bit...
                renderImage.pixel(x, y) += colorPixelSample / samplesPerPixel;

            }

        }

        // ----------------------------------------------------

    }

    // ========================================================

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
