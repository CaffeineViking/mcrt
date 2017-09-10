#include "mcrt/camera.hh"

mcrt::Camera::Camera(const glm::dvec3& viewPlanePosition, const glm::dvec3& lookAtPosition,
                     const glm::dvec3& upwardVector, double aspectRatio, double fieldOfView) {
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
