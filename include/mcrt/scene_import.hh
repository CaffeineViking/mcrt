#ifndef MCRT_SCENE_IMPORT_HH
#define MCRT_SCENE_IMPORT_HH

#include <string>
#include "mcrt/scene.hh"

namespace mcrt {
    class SceneImporter {
    public:
        static Scene load(const std::string&);
    };
}

#endif
