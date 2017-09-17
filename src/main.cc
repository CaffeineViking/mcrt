#include <chrono>
#include <cstring>
#include <iostream>
#include <iomanip>

#include "mcrt/param_import.hh"
#include "mcrt/scene_import.hh"
#include "mcrt/parameter.hh"
#include "mcrt/scene.hh"

#include "mcrt/image.hh"
#include "mcrt/image_export.hh"

int usage(int argc, char** argv) {
    if (argc < 2) std::cerr << "Error: need the path to render scenes to!" << std::endl;
    std::cerr << "Usage: " << argv[0] << " "
              << "IMAGE [SCENE] [PARAMETER]"
              << std::endl;
    return 1;
}

void printProgress(const std::string& task, double progress, size_t characters = 70) {
    std::cout << task << " [";
    size_t position = progress * characters;
    for (size_t i { 0 }; i < characters; ++i) {
        if (i < position) std::cout << "=";
        else if (i > position) std::cout << " ";
        else std::cout << ">";
    } std::cout << "] ";
    size_t percent = progress * 100.0;
    std::cout << percent << " %";
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

    // Note that render background will be transparent.
    mcrt::Image renderImage { parameters.resolutionWidth,
                              parameters.resolutionHeight };

    auto renderStart { std::chrono::steady_clock::now() };

    double renderProgress { 0.0 };

    printProgress("Ray tracing:", renderProgress);

    std::cout << std::endl; // Reset b4 progress bar hack.
    auto renderFinish { std::chrono::steady_clock::now() };
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
