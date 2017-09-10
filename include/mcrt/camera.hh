#ifndef MCRT_CAMERA_HH
#define MCRT_CAMERA_HH

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "mcrt/image.hh"
#include "mcrt/color.hh"

namespace mcrt {
    class Camera {
    public:
        // Create a camera with the default settings, a aspect ratio
        // of 1:1 and a horizontal field of view of 90 degrees, yup.
        Camera(const glm::dvec3& position, const glm::dvec3& lookAt)
            : Camera { position, lookAt, { 0.0, 1.0, 0.0 }, 1.0,
                       glm::half_pi<double>() } {}

        // Complete constructor with bells and whistles. It should uniquely specify some
        // camera by using position, focus position and deriving eye position from hFov.
        Camera(const glm::dvec3&, const glm::dvec3&, const glm::dvec3&, double, double);

        void   setAspectRatio(double);
        double getAspectRatio() const;
        double getFieldOfView() const;
        void   setFieldOfView(double);

        // Also ordered clockwise, like [ tl, tr, br, bl ]...
        struct SamplingPlane { glm::dvec3 planeCorners[4]; };
        // We use this function to find out where we should sample a pixel under
        // (x, y) in the view plane. This results in a continuous sampling area.
        SamplingPlane getPixelSamplingPlane(const Image&, size_t, size_t) const;

        void moveTo(const glm::dvec3&);
        glm::dvec3 getViewPlanePosition() const;
        void lookAt(const glm::dvec3&, const glm::dvec3&);

    private:
        glm::dvec3 eyePoint;
        // Ordered in clockwise.
        // i.e. [tl, tr, br, bl]
        glm::dvec3 viewPlane[4];
    };
}

#endif
