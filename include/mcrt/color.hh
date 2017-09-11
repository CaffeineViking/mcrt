#ifndef MCRT_COLOR_HH
#define MCRT_COLOR_HH

#include <iostream>

namespace mcrt {
    template<typename T>
    struct Color {
        Color<T>& operator*=(double);
        Color<T>& operator/=(double);
        Color<T>& operator+=(const Color<T>&);
        Color<T>& operator-=(const Color<T>&);
        Color<T>& operator*=(const Color<T>&);
        Color<T>& operator/=(const Color<T>&);

        explicit operator char*();
        explicit operator const char*() const;
        T r, g, b, a; // RGBA-colorspace data.
    };

    // This is quite a lot of boilerplate, but the interface is quite nice.
    // If glm had support for unsig char vec3 them we might have used that.

    template<typename T> bool operator==(const Color<T>&, const Color<T>&);
    template<typename T> bool operator!=(const Color<T>&, const Color<T>&);

    template<typename T> Color<T> operator+(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator-(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator*(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator/(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator*(double, const Color<T>&);
    template<typename T> Color<T> operator/(double, const Color<T>&);

    // TODO:  remove this when we are done with the preliminary stuff, not needed.
    // Useful for debugging color related problems when ray-tracing the radiances.
    template<typename T> std::ostream& operator<<(std::ostream&, const Color<T>&);
}

template<typename T>
mcrt::Color<T>::operator char*() {
    // Assume struct have byte alignment.
    return reinterpret_cast<char*>(this);
}

template<typename T>
mcrt::Color<T>::operator const char*() const {
    // Assume structs have byte-byte alignment.
    return reinterpret_cast<const char*>(this);
}

template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator+=(const Color<T>& other) {
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    this->a += other.a;
    return *this;
}

template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator-=(const Color<T>& other) {
    this->r -= other.r;
    this->g -= other.g;
    this->b -= other.b;
    this->a -= other.a;
    return *this;
}

template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator*=(const Color<T>& other) {
    this->r *= other.r;
    this->g *= other.g;
    this->b *= other.b;
    this->a *= other.a;
    return *this;
}

template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator*=(double scale) {
    this->r *= scale;
    this->g *= scale;
    this->b *= scale;
    this->a *= scale;
    return *this;
}

template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator/=(const Color<T>& other) {
    this->r /= other.r;
    this->g /= other.g;
    this->b /= other.b;
    this->a /= other.a;
    return *this;
}
template<typename T>
mcrt::Color<T>& mcrt::Color<T>::operator/=(double scale) {
    this->r /= scale;
    this->g /= scale;
    this->b /= scale;
    this->a /= scale;
    return *this;
}

template<typename T>
bool mcrt::operator==(const Color<T>& left, const Color<T>& right) {
    return left.r == right.r && left.g == right.g &&
           left.b == right.b && left.a == right.a;
}

template<typename T>
bool mcrt::operator!=(const Color<T>& left, const Color<T>& right) {
    return !(left == right);
}

template<typename T>
mcrt::Color<T> mcrt::operator+(const Color<T>& left, const Color<T>& right) {
    Color<T> result { left };
    result += right;
    return result;
}

template<typename T>
mcrt::Color<T> mcrt::operator-(const Color<T>& left, const Color<T>& right) {
    Color<T> result { left };
    result -= right;
    return result;
}

template<typename T>
mcrt::Color<T> mcrt::operator*(const Color<T>& left, const Color<T>& right) {
    Color<T> result { left };
    result *= right;
    return result;
}

template<typename T>
mcrt::Color<T> mcrt::operator*(double left, const Color<T>& right) {
    Color<T> result { right };
    result *= left;
    return result;
}

template<typename T>
mcrt::Color<T> mcrt::operator/(const Color<T>& left, const Color<T>& right) {
    Color<T> result { left };
    result /= right;
    return result;
}

template<typename T>
mcrt::Color<T> mcrt::operator/(double left, const Color<T>& right) {
    Color<T> result { right };
    result /= left;
    return result;
}

template<typename T>
std::ostream& mcrt::operator<<(std::ostream& output, const Color<T>& color) {
    return output << "color(" << static_cast<double>(color.r) << ", " << static_cast<double>(color.g)
                  << ", "  << static_cast<double>(color.b) << ", " << static_cast<double>(color.a) << ")";
}

#endif
