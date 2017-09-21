#ifndef MCRT_COLOR_HH
#define MCRT_COLOR_HH

namespace mcrt {
    template<typename T>
    struct Color {
        Color() = default;
        Color(const glm::dvec3& v) {
            r = v.r * 255;
            g = v.g * 255;
            b = v.b * 255;
            a = 255;
        }
        Color(T rr, T gg, T bb, T aa): r{rr}, g{gg}, b{bb}, a{aa} 
        {}
            
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
    template<typename T> bool operator<(const Color<T>&, const Color<T>&);
    
    template<typename T> Color<T> operator+(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator-(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator*(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator/(const Color<T>&, const Color<T>&);
    template<typename T> Color<T> operator*(double, const Color<T>&);
    template<typename T> Color<T> operator/(double, const Color<T>&);
    template<typename T> Color<T> operator/(const Color<T>&, double);
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
bool mcrt::operator<(const Color<T>& c1, const Color<T>&c2) {
    double len2_c1 = ((c1.r * c1.r) + (c1.g * c1.g) + (c1.b * c1.b) + (c1.a * c1.a));
    double len2_c2 = ((c2.r * c1.r) + (c2.g * c2.g) + (c2.b * c2.b) + (c2.a * c2.a));
    return len2_c1 < len2_c2;
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
mcrt::Color<T> mcrt::operator/(const Color<T>& right,double left) {
    Color<T> result { right };
    result /= left;
    return result;
}
#endif
