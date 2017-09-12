#include "mcrt/camera.hh"

#include <glm/gtx/string_cast.hpp>

mcrt::Camera::Camera(const glm::dvec3& viewPlanePosition, const glm::dvec3& lookAtPosition,
                     const glm::dvec3& upwardVector, double aspectRatio, double fieldOfView) {
    // Initial, temporary values.
    viewPlane[0] = { -1, +1, 0 };
    viewPlane[1] = { +1, +1, 0 };
    viewPlane[2] = { +1, -1, 0 };
    viewPlane[3] = { -1, -1, 0 };
    eyePoint = { 0.0, 0.0, 1.0 };

    // Setup our camera setting.
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
    double eyeToPlaneDistance { glm::distance(eyePoint, getViewPlanePosition()) };
    double viewPlaneWidth     { glm::distance(viewPlane[1], viewPlane[0]) };
    return 2*glm::atan((viewPlaneWidth/2)/eyeToPlaneDistance);
}

void   mcrt::Camera::setFieldOfView(double fieldOfView) {
}

glm::dvec3 mcrt::Camera::getPixelCenter(const Image& image, size_t i, size_t j) const {
    // Fetch the axes built from the top-left to top-right (x)
    // and the top-left to bottom-left (y-axis). This is to do
    // mirroring of the coordinate system found in the images.
    glm::dvec3 xViewPlaneAxis { viewPlane[1] - viewPlane[0] },
               yViewPlaneAxis { viewPlane[3] - viewPlane[0] };

    // We find the lengths of the view plane's basis axes.
    double viewPlaneWidth  { glm::length(xViewPlaneAxis) },
           viewPlaneHeight { glm::length(yViewPlaneAxis) };

    xViewPlaneAxis = glm::normalize(xViewPlaneAxis);
    yViewPlaneAxis = glm::normalize(yViewPlaneAxis);

    // Finally, we compute the scaling ratio needed from pixel coordinates to viewplane.
    double widthScaleRatio  { viewPlaneWidth  / static_cast<double>(image.getWidth()) },
           heightScaleRatio { viewPlaneHeight / static_cast<double>(image.getHeight()) };

    glm::dvec3 pixelCenter { viewPlane[0] };
    // Offset pixel by "half a pixel" for center.
    double pixelX { i + widthScaleRatio  / 2.0 },
           pixelY { j + heightScaleRatio / 2.0 };
    // Finally, finding the centers of the pixel in the plane.
    pixelCenter += pixelX * widthScaleRatio  * xViewPlaneAxis;
    pixelCenter += pixelY * heightScaleRatio * yViewPlaneAxis;
    return pixelCenter;
}

mcrt::Camera::SamplingPlane mcrt::Camera::getPixelSamplingPlane(const Image& image, size_t i, size_t j) const {
    // Fetch the axes built from the top-left to top-right (x)
    // and the top-left to bottom-left (y-axis). This is to do
    // mirroring of the coordinate system found in the images.
    glm::dvec3 xViewPlaneAxis { viewPlane[1] - viewPlane[0] },
               yViewPlaneAxis { viewPlane[3] - viewPlane[0] };

    // We find the lengths of the view plane's basis axes.
    double viewPlaneWidth  { glm::length(xViewPlaneAxis) },
           viewPlaneHeight { glm::length(yViewPlaneAxis) };

    xViewPlaneAxis = glm::normalize(xViewPlaneAxis);
    yViewPlaneAxis = glm::normalize(yViewPlaneAxis);

    // Finally, we compute the scaling ratio needed from pixel coordinates to viewplane.
    double widthScaleRatio  { viewPlaneWidth  / static_cast<double>(image.getWidth()) },
           heightScaleRatio { viewPlaneHeight / static_cast<double>(image.getHeight()) };

    SamplingPlane pixelSamplingPlane;
    pixelSamplingPlane.corners[0] = viewPlane[0];
    // Top-left pixel corner is scaled (i,j) units away from view plane tl.
    pixelSamplingPlane.corners[0] += i * widthScaleRatio  * xViewPlaneAxis;
    pixelSamplingPlane.corners[0] += j * heightScaleRatio * yViewPlaneAxis;

    // The rest are simply one pixel length away (or through the diagonal) from top-left pixel corners.
    pixelSamplingPlane.corners[1] =  pixelSamplingPlane.corners[0] + widthScaleRatio  * xViewPlaneAxis;
    pixelSamplingPlane.corners[2] =  pixelSamplingPlane.corners[0] + widthScaleRatio  * xViewPlaneAxis;
    pixelSamplingPlane.corners[2] += heightScaleRatio * yViewPlaneAxis; // top-left ----> bottom-right.
    pixelSamplingPlane.corners[3] =  pixelSamplingPlane.corners[0] + heightScaleRatio * yViewPlaneAxis;
    return pixelSamplingPlane;
}

void mcrt::Camera::moveTo(const glm::dvec3& viewPlanePosition) {
    glm::dvec3 planeCenter  { getViewPlanePosition() };
    glm::dvec3 eyeToCenter { planeCenter - eyePoint };
    glm::dvec3 tl { viewPlane[0] - planeCenter },
               tr { viewPlane[1] - planeCenter },
               br { viewPlane[2] - planeCenter },
               bl { viewPlane[3] - planeCenter };

    // Calculate new positions based on center.
    eyePoint = viewPlanePosition - eyeToCenter;
    viewPlane[0] = viewPlanePosition + tl;
    viewPlane[1] = viewPlanePosition + tr;
    viewPlane[2] = viewPlanePosition + br;
    viewPlane[3] = viewPlanePosition + bl;
}

glm::dvec3 mcrt::Camera::getEyePosition() const {
    // Now this one is quite easy!
    return eyePoint;
}

glm::dvec3 mcrt::Camera::getViewPlanePosition() const {
    glm::dvec3 viewDiagonal { viewPlane[2] - viewPlane[0] };
    // Return the center of the view plane accross diagonal.
    return viewPlane[0] + 0.5*viewDiagonal;
}

void mcrt::Camera::lookAt(const glm::dvec3& lookAtPosition, const glm::dvec3& upwardVector) {
    double eyeToPlaneDistance = glm::distance(eyePoint, getViewPlanePosition());
    glm::dvec3 w = glm::normalize(lookAtPosition - eyePoint);
    glm::dvec3 u = glm::cross(w, upwardVector);
    glm::dvec3 v = glm::cross(u, w);
    glm::dvec3 viewPlaneCenter = eyePoint + w*eyeToPlaneDistance;
	      
    viewPlane[0] = viewPlaneCenter - u + v;
    viewPlane[1] = viewPlaneCenter + u + v;
    viewPlane[2] = viewPlaneCenter + u - v;
    viewPlane[3] = viewPlaneCenter + u + v;
}

std::ostream& mcrt::operator<<(std::ostream& output, const Camera& camera) {
    return output << "eye: " << glm::to_string(camera.eyePoint) << std::endl
                  << "top-left:     " << glm::to_string(camera.viewPlane[0]) << std::endl
		  << "top-right:    " << glm::to_string(camera.viewPlane[1]) << std::endl
                  << "bottom-right: " << glm::to_string(camera.viewPlane[2]) << std::endl
                  << "bottom-left:  " << glm::to_string(camera.viewPlane[3]) << std::endl
                  << "aspectRatio: " << camera.getAspectRatio()
                  << " fieldOfView: " << camera.getFieldOfView() <<  std::endl;
}
