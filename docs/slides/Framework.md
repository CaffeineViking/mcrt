Header

Introduction
    * Table of content

Article topics
    * What's the paper about?
    * Global Illumniation
        * Why?
            * Rendering equation
            * Fully rendered scenes with one model
        * How?
            * Radisoity
                + Viewport independent
                - Diffuse illumination only
                + No noise
            * Whitted
                + Specular and Diffuse surfaces
                    + Refractive and Refflective
                - Viewport dependent
                - Basic lighting situations
                - Aliasing
                - Slow
            * Path-tracing
                + Color bleeding
                + Soft shadows
                + Complex lighting situations
                - Super slow
    * Why path-tracing with photon mapping?
        + Easier Caustics
        + Faster convergence
        + Volumetric surface properties
        - Memory requirements
        + Partially viewport dependent
    * Paper overview
    * Concepts
        * Building photon map (Pass 1)
            * Light source
            * Depends on surface material
            * Global 
            * Caustic (Why not Monte Carlo sampling?)
            * Voulume
            * KD-tree
            * Optimzation
                * Projection map
        * How path-tracing and photon maping integrates (Pass 2)
            * N - closes photons - filtered
            * Split BRDF in 3 parts, 
            * Direct illumination, 
            * Caustics, 
            * Indirect illumination
            * Participating medias - use volume instead of surface
Discussion
    * Summary
Future studies
    * How can the project be expanded?
    * Photon splating
    * Photon relaxation
    * Progressive photon mapping
