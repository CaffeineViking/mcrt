#ifndef MCRT_PARAM_IMPORT_HH
#define MCRT_PARAM_IMPORT_HH

#include <string>
#include "mcrt/image.hh"
#include "mcrt/parameter.hh"

namespace mcrt {
    class ParameterImporter {
    public:
        static Parameters load(const std::string&);
    };
}

#endif
