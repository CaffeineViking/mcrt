#include "mcrt/camera.hh"

mcrt::Camera::Camera(const glm::dvec3& position,  const glm::dvec3& lookAtPosition,
                     const glm::dvec3& upwardVector, double aspectRatio, double fieldOfView) {
    // Initial, temporary values.
    viewPlane[0] = { -1, +1, 0 };
    viewPlane[1] = { +1, +1, 0 };
    viewPlane[2] = { +1, -1, 0 };
    viewPlane[3] = { -1, -1, 0 };
    eyePoint = { -1.0, 0.0, 0.0 };

    // Setup our camera setting.
    setAspectRatio(aspectRatio);
    setFieldOfView(fieldOfView);
    moveTo(position);

    // Does the order matter here or not?
    lookAt(lookAtPosition, upwardVector);
}

void   mcrt::Camera::setAspectRatio(double aspectRatio) {
    double viewPlaneAspectRatio { getAspectRatio() };
    glm::dvec3 viewPlaneCenter { getViewPlanePosition() };
    glm::dvec3 xViewPlaneAxis { viewPlane[1] - viewPlane[0] },
               yViewPlaneAxis { viewPlane[3] - viewPlane[0] };
    double aspectRatioDifference { aspectRatio / viewPlaneAspectRatio };
    aspectRatioDifference = std::sqrt(aspectRatioDifference);

    xViewPlaneAxis *= aspectRatioDifference;
    yViewPlaneAxis *= 1.0 / aspectRatioDifference;
    glm::dvec3 xHalfAxis { xViewPlaneAxis / 2.0 },
               yHalfAxis { yViewPlaneAxis / 2.0 };

    viewPlane[0] = viewPlaneCenter - xHalfAxis - yHalfAxis;
    viewPlane[1] = viewPlaneCenter + xHalfAxis - yHalfAxis;
    viewPlane[2] = viewPlaneCenter + xHalfAxis + yHalfAxis;
    viewPlane[3] = viewPlaneCenter - xHalfAxis + yHalfAxis;
}

double mcrt::Camera::getAspectRatio() const {
    // Just fetch the length of each axis and find the ratio of these.
    double viewPlaneWidth  { glm::distance(viewPlane[1], viewPlane[0]) },
           viewPlaneHeight { glm::distance(viewPlane[3], viewPlane[0]) };
    return viewPlaneWidth / viewPlaneHeight;
}

double mcrt::Camera::getFieldOfView() const {
    double eyeToPlaneDistance { glm::distance(eyePoint, getViewPlanePosition()) };
    double viewPlaneWidth     { glm::distance(viewPlane[1], viewPlane[0]) };
    return 2.0 * glm::atan((viewPlaneWidth / 2.0) / eyeToPlaneDistance);
}

void   mcrt::Camera::setFieldOfView(double fieldOfView) {
    double viewPlaneWidth { glm::distance(viewPlane[1], viewPlane[0]) };
    double focalDistance { (viewPlaneWidth/2.0) / std::tan(fieldOfView/2.0) };
    glm::dvec3 eyeNormal { -glm::normalize(getViewPlanePosition()-eyePoint) };
    eyePoint = getViewPlanePosition() + eyeNormal * focalDistance;
}

glm::dvec3 mcrt::Camera::getPixelCenter(const Image& image, size_t i, size_t j) const {
    // Fetch the plane of the pixel, and just get the pixel's central position.
    SamplingPlane pixelSamplingPlane  { getPixelSamplingPlane(image,  i,  j) };
    // That is basically just half the diagonal from the top-left to the bottom-right corner.
    glm::dvec3 diagonal { (pixelSamplingPlane.corners[2]-pixelSamplingPlane.corners[0])/2. };
    return pixelSamplingPlane.corners[0] + diagonal; // From the top-left to the center area.
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
    glm::dvec3 planeToCamera { eyePoint - getViewPlanePosition() };
    glm::dvec3 viewPlaneCenter { getViewPlanePosition() };

    glm::dvec3 tl { viewPlane[0] - viewPlaneCenter },
               tr { viewPlane[1] - viewPlaneCenter },
               br { viewPlane[2] - viewPlaneCenter },
               bl { viewPlane[3] - viewPlaneCenter };

    viewPlane[0] = viewPlanePosition + tl;
    viewPlane[1] = viewPlanePosition + tr;
    viewPlane[2] = viewPlanePosition + br;
    viewPlane[3] = viewPlanePosition + bl;
    eyePoint = viewPlanePosition + planeToCamera;
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
    glm::dvec3 viewPlaneCenter = getViewPlanePosition();
    double eyeToPlaneDistance = glm::distance(eyePoint, viewPlaneCenter);
    double eyeFieldOfView { getFieldOfView() }; // For bootstrapping hFoV later.

    glm::dvec3 w = glm::normalize(lookAtPosition - viewPlaneCenter);
    glm::dvec3 u = glm::cross(w, upwardVector);
    glm::dvec3 v = glm::cross(u, w);
    eyePoint = viewPlaneCenter - w*eyeToPlaneDistance;

    viewPlane[0] = viewPlaneCenter - u + v;
    viewPlane[1] = viewPlaneCenter + u + v;
    viewPlane[2] = viewPlaneCenter + u - v;
    viewPlane[3] = viewPlaneCenter - u - v;

    if (glm::dot(eyePoint, lookAtPosition - getViewPlanePosition()) > 0) eyePoint = -eyePoint;
    setFieldOfView(eyeFieldOfView); // We moved the eye to some arbitrary position, fix fov...
}
