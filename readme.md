Monte Carlo Raytracer from Scratch
==================================

<p align="center">
    <img src="/docs/share/new_render.png" alt="Render 1024x1024 1024 SPP"/>
    <img width=33% src="/docs/share/cornell_local_illumination.png" alt="Cornell Box  - Local Illumination"/>
    <img width=33% src="/docs/share/cornell_global_illumination.png" alt="Cornell Box - Global Illumination"/>
    <img width=33% src="/docs/share/whitted_raytracing.png" alt="Utah Teapot"/>
</p>

A *Monte Carlo raytracer* produces *photorealistic images* of given scenes (given good assets and enough time to fully converge). It's a technique which allows *global illumination*, giving optical effects such as *color bleeding*, *hard and soft shadows* and *caustics*. In this repository you'll find a full Monte Carlo raytracer implementation written in modern C++, along with the accompaying paper [*Monte Carlo Raytracing from Scratch*](https://caffeineviking.net/share/papers/mcrt.pdf) which describes the theory and practical details needed to both understand and implement your own raytracer, along with some benchmarks, reflections and future work. Our raytracer is written from the ground up, and doesn't need any libraries to be linked. We've used the header only libraries: *g-truc/glm* (for vector and matrix operations), *nlohmann/json* (for our scene loader) and *syoyo/tinyobjloader* (for loading meshes).

Below you'll find a non-exhaustive list of features:

Features
--------

* **Written in modern C++ (11/14)**
* **Scene and parameter loading**
    * Using header JSON library
* **Loadable triangle meshes**
   * Using header only lib.
* **Render Parallelization**
    * Using `OpenMP`
    * Progressive rendering
* **Ray-surface intersections**
    * For parametric spheres
    * For triangles (using Möller–Trumbore)
    * For arbitrary meshes (with sphere BV)
* **Surface reflection properties**
    * Lambertian reflection model
    * Oren–Nayar reflection model
* **Direct light contributions**
    * Using point light sources
    * Using any area light sources
    * With Monte Carlo integration
* **Importance sampling**
    * By cosine-weights
* **Indirect light contributions**
    * By specular reflection
    * By specular refractions
        * with Fresnel factor
        * and Snell's law
    * By diffuse reflections
        * color bleeding!
    * Russian roulette
* **Photon mapping**
    * In balanced k-d tree
    * Direct light radiance estimation
        * by sampling fixed sphere
        * cone-filtered estimation
* **Anti-aliasing by supersampling**
    * Using the grid pattern
    * Using some random pattern
* [**Paper showing techniques**](https://caffeineviking.net/share/papers/mcrt.pdf)
    * [**Photon mapping slides**](https://caffeineviking.net/share/papers/giph.pdf)

Compiling and Testing
---------------------

1. Place yourself in the root directory of this project.
2. Acquire the latest version of the `premake5` build system.
3. Thereafter, execute `premake5 gmake` if building on Make.
4. Finally, issue the command `make -j8 -C build` and wait.
5. When complete, you'll find the built software in `bin`.
6. **Shortcuts:** `make render` and `make view-render`.

Usage and Documents
-------------------

* `bin/mcrt <image-file> [<scene-file> <param-file>]`: render scene in `<scene-file>` with the raytracer parameters in `<param-file>` to an image file `<image-file>` using a supported format (ppm, ff and png). Uses defaults if not given.
* `make render` and `make view-render`: builds the project and render `share/scene.json` with `share/param.json`. Can be changed to something else by looking at the `Makefile`. Uses the `premake5` build system; make sure to have that :). It also opens your image `share/render.png` with `feh` and continuously updates when additional details are rendered.
* `make profile` and `make view-profile`: produces *flame graphs* by profiling with `perf`.
* `make docs`: produces the report *Monte Carlo Raytracing from Scratch* for `mcrt`.
* `utils/photon-map.r`: gives a visualization of the directly built photon map.
* `utils/png-distance.r`: takes in two images, produce difference between them.
* `utils/PKGBUILD`: use `makepkg -i` to install this when you are running Arch Linux.

System Requirements
-------------------

Should run on anything, even on toaster-like potato-ish hardware.

Dependencies
------------

Fortunately, there aren't any dependencies besides `premake` now.

Structure
---------

* `bin`: contains the built software and accompanying testing suite.
* `build`: stores intermediate object files and generated GNU Make files.
    * `obj`: has all of the generated object files given under compilation.
    * `Makefile`: automatically generated by executing `premake5 gmake`.
    * `*.make`: program specific make config for augmenting `Makefile`.
* `docs`: any generated documentation for this project is over here.
* `include`: both external and internal project headers are here.
    * `project directories`: internal headers for the project.
* `license.md`: please look through this very carefully.
* `premake5.lua`: configuration file for the build system.
* `readme.md`: this file, contains information on the project.
* `share`: any extra data that needs to be bundled should be here.
* `src`: all source code for the project should be located below here.
    * `project directories`: source code for specific project build.
* `utils`: any sort of helper scripts or similar should be over here.

Contributing
------------

1. See if there any updates upstream: `git remote update`
2. If you are already working on a feature branch, jump to step 4.
3. Create a new branch for your feature: `git branch <feature>`
4. Change to the feature branch by: `git checkout <feature>`
5. Start/continue building the feature. Jump to 7 if done.
6. Push your changes to your remote feature branch with:
   * `git add -A` (everything) or `git add -u` (updates).
   * `git commit -m "Message describing the additions."`
   * Publish remotely: `git push origin <feature>`
   * Go back to step 1 or 5 (if working quickly).
7. Your feature is done (single/many commits).
8. Fetch the changes from upstream first:
   * Go to master: `git checkout -b master`
   * Pull changes: `git pull origin master`
8. Rebase your changes on top of `master`:
   * Back to feature: `git checkout <feature>`
   * Thereafter, issue: `git rebase master`
   * Fix any conflicts that may happen...
   * Changes now on top of the Git tree.
9. Finally, go to GitHub and issue PR.
   * Listen in on other's feedback!
   * Make changes if necessary.
10. Back to the master: `git checkout master`
11. Pat yourself on the back, then go back to 1 again.

Reporting Bugs
--------------

There are definitely no known bugs in this software at this time.
