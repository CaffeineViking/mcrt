#ifndef MCRT_INTERPOLANTS_HH
#define MCRT_INTERPOLANTS_HH

namespace mcrt {
    namespace interpolants {
        template<typename T, typename W = double>
        T linear(const T& first, const T& second, W weight) {
            return (1 - weight)*first + weight*second;
        }

        template<typename T, typename W = double>
        T bilinear(const T& tl, const T& tr, const T& br,
                   const T& bl, W xWeight, W yWeight) {
            T first   { linear(tl, tr, xWeight) },
              second  { linear(bl, br, xWeight) };
            return linear(first, second, yWeight);
        }
    }
}

#endif
