#include <iostream>

#include "mcrt/mcrt.hh"
#include "mcrt/color.hh"
#include "mcrt/image.hh"
#include "mcrt/camera.hh"
#include "mcrt/sphere.hh"
#include "mcrt/triangle.hh"
#include "mcrt/scene.hh"
#include "mcrt/image_export.hh"
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <sstream>

int main(int, char**) {

    mcrt::Scene scene{};
    mcrt::Material red{glm::dvec3(1.0,0.0,0.0)};
    mcrt::Material yellow{glm::dvec3(1.0,1.0,0.0)};
    mcrt::Material green{glm::dvec3(0.0,1.0,0.0)};
    mcrt::Material cyan{glm::dvec3(0.0,1.0,1.0)};
    mcrt::Material blue{glm::dvec3(0.0,0.0,1.0)};
    mcrt::Material magenta{glm::dvec3(1.0,0.0,1.0)};
    mcrt::Material white{glm::dvec3(1.0,1.0,1.0)};
    mcrt::Material grey{glm::dvec3(0.7,0.7,0.7)};

    mcrt::Material ceiling{white};
    
    mcrt::Triangle wall11{
        glm::dvec3(0,5,-6),
        glm::dvec3(10,5,-6),
        glm::dvec3(0,-5,-6),
        cyan
    };
    mcrt::Triangle wall12{
        glm::dvec3(0,-5,-6),
        glm::dvec3(10,5,-6),
        glm::dvec3(10,-5,-6),
        cyan
    };

    scene.add(wall11);
    scene.add(wall12);

    mcrt::Triangle wall21{
        glm::dvec3(0,5,-6),
        glm::dvec3(-3,5,0),
        glm::dvec3(0,-5,-6),
        blue
    };
    mcrt::Triangle wall22{
        glm::dvec3(0,-5,-6),
        glm::dvec3(-3,-5,0),
        glm::dvec3(-3,5,0),
        blue
    };

    scene.add(wall21);
    scene.add(wall22);

    mcrt::Triangle wall31{
        glm::dvec3(-3,5,0),
        glm::dvec3(0,5,6),
        glm::dvec3(-3,-5,0),
        magenta
    };
    mcrt::Triangle wall32{
        glm::dvec3(-3,-5,0),
        glm::dvec3(0,-5,6),
        glm::dvec3(0,5,6),
        magenta
    };

    scene.add(wall31);
    scene.add(wall32);

    mcrt::Triangle wall41{
        glm::dvec3(0,5,6),
        glm::dvec3(10,5,6),
        glm::dvec3(10,-5,6),
        red
    };
    mcrt::Triangle wall42{
        glm::dvec3(10,-5,6),
        glm::dvec3(0,-5,6),
        glm::dvec3(0,5,6),
        red
    };

    scene.add(wall41);
    scene.add(wall42);

    mcrt::Triangle wall51{
        glm::dvec3(10,5,6),
        glm::dvec3(13,5,0),
        glm::dvec3(13,-5,0),
        yellow
    };
    mcrt::Triangle wall52{
        glm::dvec3(13,-5,0),
        glm::dvec3(10,-5,6),
        glm::dvec3(10,5,6),
        yellow
    };

    scene.add(wall51);
    scene.add(wall52);

    mcrt::Triangle wall61{
        glm::dvec3(13,5,0),
        glm::dvec3(10,5,-6),
        glm::dvec3(10,-5,-6),
        green
    };
    mcrt::Triangle wall62{
        glm::dvec3(10,-5,-6),
        glm::dvec3(13,-5,0),
        glm::dvec3(13,5,0),
        green
    };

    scene.add(wall61);
    scene.add(wall62);
    
    mcrt::Triangle ceiling1{
        glm::dvec3(5,5,0),
        glm::dvec3(0,5,6),
        glm::dvec3(10,5,6),
        ceiling
    };

    mcrt::Triangle ceiling2{
        glm::dvec3(5,5,0),
        glm::dvec3(10,5,6),
        glm::dvec3(13,5,0),
        ceiling
    };

    mcrt::Triangle ceiling3{
        glm::dvec3(5,5,0),
        glm::dvec3(13,5,0),
        glm::dvec3(10,5,-6),
        ceiling
    };

    mcrt::Triangle ceiling4{
        glm::dvec3(5,5,0),
        glm::dvec3(0,5,-6),
        glm::dvec3(10,5,-6),
        ceiling
    };

    mcrt::Triangle ceiling5{
        glm::dvec3(5,5,0),
        glm::dvec3(0,5,-6),
        glm::dvec3(-3,5,0),
        ceiling
    };

    mcrt::Triangle ceiling6{
        glm::dvec3(5,5,0),
        glm::dvec3(-3,5,0),
        glm::dvec3(0,5,6),
        ceiling
    };

    scene.add(ceiling1);    
    scene.add(ceiling2);    
    scene.add(ceiling3);    
    scene.add(ceiling4);    
    scene.add(ceiling5);
    scene.add(ceiling6);

    mcrt::Triangle floor1{
        glm::dvec3(5,-5,0),
        glm::dvec3(0,-5,6),
        glm::dvec3(10,-5,6),
        ceiling
    };

    mcrt::Triangle floor2{
        glm::dvec3(5,-5,0),
        glm::dvec3(10,-5,6),
        glm::dvec3(13,-5,0),
        ceiling
    };

    mcrt::Triangle floor3{
        glm::dvec3(5, -5,0),
        glm::dvec3(13,-5,0),
        glm::dvec3(10,-5,-6),
        ceiling
    };

    mcrt::Triangle floor4{
        glm::dvec3(5, -5,0),
        glm::dvec3(0, -5,-6),
        glm::dvec3(10,-5,-6),
        ceiling
    };

    mcrt::Triangle floor5{
        glm::dvec3(5, -5,0),
        glm::dvec3(0, -5,-6),
        glm::dvec3(-3,-5,0),
        ceiling
    };

    mcrt::Triangle floor6{
        glm::dvec3(5, -5,0),
        glm::dvec3(-3,-5,0),
        glm::dvec3(0, -5,6),
        ceiling
    };

    scene.add(floor1);    
    scene.add(floor2);    
    scene.add(floor3);    
    scene.add(floor4);    
    scene.add(floor5);
    scene.add(floor6);

    mcrt::Material sphereMaterial {grey};
    mcrt::Sphere sphere {glm::dvec3(7.0,0.0,0.0),1.0, sphereMaterial};
    scene.add(sphere);

    mcrt::Light light{glm::dvec3(10.0,0.0,0.0),glm::dvec3(1.0)};
    scene.add(light);

    mcrt::Image renderImage { 512, 512 };
    renderImage.clear({0, 0, 0,  255});

    std::cout << "Image size: " << renderImage.getWidth() << "x" << renderImage.getHeight() << std::endl;
    std::cout << "Image aspect ratio: " << renderImage.getAspectRatio() << std::endl;

    mcrt::Color<unsigned char> firstPixelColor { renderImage.pixel(0, 0) };
    std::cout << "First pixel color: (" << static_cast<unsigned>(firstPixelColor.r) << ", "
                                        << static_cast<unsigned>(firstPixelColor.g) << ", "
                                        << static_cast<unsigned>(firstPixelColor.b) << ", "
                                        << static_cast<unsigned>(firstPixelColor.a)
                                        << ")" << std::endl;

    std::cout << "Replacing image with a nice gradient." << std::endl;

    double stepX = 255.0 / renderImage.getWidth(),
           stepY = 255.0 / renderImage.getHeight();
    for (size_t y { 0 }; y < renderImage.getHeight(); ++y) {
        for (size_t x { 0 }; x < renderImage.getWidth(); ++x) {
            renderImage.pixel(x, y) = { x*stepX, y*stepY,
                                        0, 255 };
        }
    }

    // Create camera with standard settings, position is rela-
    // tive to the eye position (as are all of the operaitons)
    // but not the viewport ones (like finding pixels & stuff.
    mcrt::Camera camera { {0.0, 0.0, 0.0}, {1.0, 0.0,0.0} };

    // camera.setAspectRatio(image.getAspectRatio());
    // camera.setFieldOfView(3.141592 / 2.0);
    // camera.moveTo({ 1.0, 0.0, 1.0 });
    // camera.lookAt({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});

     glm::dvec3 eye { camera.getEyePosition() };
    for (size_t y { 0 }; y < renderImage.getHeight(); ++y) {
        for (size_t x { 0 }; x < renderImage.getWidth(); ++x) {
            glm::dvec3 pixel { camera.getPixelCenter(renderImage, x, y) };
            // or: call getPixelSamplePlane for gathering more samples for
            // the actual pixel colors. e.g. when supersampling 4x or 16x.

            // Do raytracing schenanigans over here.
            glm::dvec3 rayDirection { pixel - eye };
            mcrt::Ray ray{pixel, rayDirection};
            glm::dvec3 color(0.0,0.0,0.0);        
            color = scene.rayTrace(ray);
            //mcrt::Intersection i = scene.intersect(ray);
            //if(i.distance > 0){
            //    color = i.material.color;
            //    color = color * (1.0/((i.distance * i.distance) / 15.0));
            //}
            
            renderImage.pixel(x,y) = {
                static_cast<unsigned char>(color.x * 255),
                static_cast<unsigned char>(color.y * 255),
                static_cast<unsigned char>(color.z * 255), 255 };
        }
    }
    //renderImage.resize(512, 512, mcrt::Image::ResizeMethod::BILINEAR);
    std::ostringstream os;
    os << "share/render";
    os << ".ppm";
    mcrt::NetpbmImageExporter::save(renderImage, os.str());

    return 0;
}
