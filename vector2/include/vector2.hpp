#pragma once

#include <string>

namespace cl
{
    template <typename T>
    class Vector2
    {
    public:
        T x, y;

        Vector2() { x = 0; y = 0; }
        Vector2(const T x, const T y) { this->x = x; this->y = y; }
        template <typename U>
        Vector2(const U x, const U y) { this->x = (T)x; this->y = (T)y; }
        template <typename U>
        operator Vector2<U>() { return {(U)x, (U)y}; }
        template <typename U>
        friend constexpr Vector2<T> operator+(Vector2<T> lhs, const Vector2<U>& rhs) { return lhs += rhs; }
        template <typename U>
        friend constexpr Vector2<T> operator-(Vector2<T> lhs, const Vector2<U>& rhs) { return lhs -= rhs; }
        template <typename U>
        friend constexpr Vector2<T> operator*(Vector2<T> lhs, const Vector2<U>& rhs) { return lhs *= rhs; }
        template <typename U>
        friend constexpr Vector2<T> operator/(Vector2<T> lhs, const Vector2<U>& rhs) { return lhs /= rhs; }
        template <typename U>
        friend constexpr Vector2<T> operator*(Vector2<T> lhs, const U& rhs) { return lhs *= rhs; }
        template <typename U>
        friend constexpr Vector2<T> operator/(Vector2<T> lhs, const U& rhs) { return lhs /= rhs; }
        template <typename U>
        constexpr Vector2<T>& operator+=(const Vector2<U>& rhs) { x += (T)rhs.x; y += (T)rhs.y; return *this; }
        template <typename U>
        constexpr Vector2<T>& operator-=(const Vector2<U>& rhs) { x -= (T)rhs.x; y -= (T)rhs.y; return *this; }
        template <typename U>
        constexpr Vector2<T>& operator*=(const Vector2<U>& rhs) { x *= (T)rhs.x; y *= (T)rhs.y; return *this; }
        template <typename U>
        constexpr Vector2<T>& operator/=(const Vector2<U>& rhs) { x /= (T)rhs.x; y /= (T)rhs.y; return *this; }
        template <typename U>
        constexpr Vector2<T>& operator*=(const U& rhs) { x *= (T)rhs; y *= (T)rhs; return *this; }
        template <typename U>
        constexpr Vector2<T>& operator/=(const U& rhs) { x /= (T)rhs; y /= (T)rhs; return *this; }
        constexpr Vector2<T> operator-() { return {-x, -y}; }
        operator std::string() const { return std::to_string(x) + ", " + std::to_string(y); }
        template <typename U>
        friend constexpr bool operator==(const Vector2<T>& lhs, const Vector2<U>& rhs) { return lhs.x == (T)rhs.x && lhs.y == (T)rhs.y; }
        template <typename U>
        friend constexpr bool operator!=(const Vector2<T>& lhs, const Vector2<U>& rhs) { return lhs.x != (T)rhs.x || lhs.y != (T)rhs.y; }
    };

    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;
    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2u;
}

struct vec_proxy
{
    long double val;
    vec_proxy operator-() { return {-val}; }
    friend cl::Vector2d operator/(const vec_proxy& lhs, const vec_proxy& rhs) { return {(double)lhs.val, (double)rhs.val}; }
    template <typename T>
    operator cl::Vector2<T>() { return {(T)val, (T)val}; }
};

inline constexpr vec_proxy operator""_v(const long double val) { return {val}; }
inline constexpr vec_proxy operator""_v(const unsigned long long int val) { return {(long double)val}; }