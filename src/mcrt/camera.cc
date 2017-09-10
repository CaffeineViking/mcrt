#include "mcrt/camera.hh"

#include <cmath>
#include <glm/gtx/string_cast.hpp>

mcrt::Camera::Camera(const glm::dvec3& viewPlanePosition, const glm::dvec3& lookAtPosition,
                     const glm::dvec3& upwardVector, double aspectRatio, double fieldOfView) {
    viewPlane[0] = { -1, +1, 0 };
    viewPlane[1] = { +1, +1, 0 };
    viewPlane[2] = { +1, -1, 0 };
    viewPlane[3] = { -1, -1, 0 };

    setAspectRatio(aspectRatio);
    setFieldOfView(fieldOfView);
    moveTo(viewPlanePosition);

    // Does the order matter here or not?
    lookAt(lookAtPosition, upwardVector);
}

void   mcrt::Camera::setAspectRatio(double aspectRatio) {
}

double mcrt::Camera::getAspectRatio() const {
}

double mcrt::Camera::getFieldOfView() const {
}

void   mcrt::Camera::setFieldOfView(double fieldOfView) {
}

mcrt::Camera::SamplingPlane mcrt::Camera::getPixelSamplingPlane(const Image& image, size_t i, size_t j) const {
}

void mcrt::Camera::moveTo(const glm::dvec3& viewPlanePosition) {
}

glm::dvec3 mcrt::Camera::getViewPlanePosition() const {
}

void mcrt::Camera::lookAt(const glm::dvec3& lookAtPosition, const glm::dvec3& upwardVector) {
}

std::ostream& mcrt::operator<<(std::ostream& output, const Camera& camera) {
    return output << "eye: " << glm::to_string(camera.eyePoint) << std::endl
                  << "top-left: " << glm::to_string(camera.viewPlane[0])
                  << " top-right: " << glm::to_string(camera.viewPlane[1])
                  << " bottom-right: " << glm::to_string(camera.viewPlane[2])
                  << " bottom-left: " << glm::to_string(camera.viewPlane[3]) << std::endl
                  << "aspectRatio: " << camera.getAspectRatio()
                  << " fieldOfView: " << camera.getFieldOfView() <<  std::endl;
}
