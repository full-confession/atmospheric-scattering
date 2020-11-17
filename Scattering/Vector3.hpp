#pragma once
#include <cmath>
#include <iostream>
#include <optional>

struct Vector3 final
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3() = default;
    Vector3(float const x, float const y, float const z)
        : x(x), y(y), z(z)
    { }

    [[nodiscard]]
    auto Length() const -> float
    {
        return std::sqrtf(x * x + y * y + z * z);
    }

    auto operator+=(Vector3 const& v) -> Vector3&
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    auto operator*=(float const v) -> Vector3&
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
};


inline auto operator*(Vector3 const& v, float const c) -> Vector3
{
    return { v.x * c, v.y * c, v.z * c };
}

inline auto operator*(float const c, Vector3 const& v) -> Vector3
{
    return v * c;
}

inline auto operator/(Vector3 const& a, float const b) -> Vector3
{
    return { a.x / b, a.y / b, a.z / b };
}

inline auto operator/(Vector3 const& a, Vector3 const& b) -> Vector3
{
    return { a.x / b.x, a.y / b.y, a.z / b.z };
}


inline auto operator+(Vector3 const& a, Vector3 const& b) -> Vector3
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline auto operator*(Vector3 const& a, Vector3 const& b) -> Vector3
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

inline auto operator-(Vector3 const& a, Vector3 const& b) -> Vector3
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline auto operator-(Vector3 const& v) -> Vector3
{
    return { -v.x, -v.y, -v.z };
}



inline auto operator<<(std::ostream& os, Vector3 const& v) -> std::ostream &
{
    os << '(' << v.x << " ," << v.y << " ," << v.z << ')';
    return os;
}

inline auto Lerp(float const a, float const b, float const t) -> float
{
    return a * (1.0f - t) + b * t;
}

inline auto Lerp(Vector3 const& a, Vector3 const& b, float const t) -> Vector3
{
    return {
        Lerp(a.x, b.x, t),
        Lerp(a.y, b.y, t),
        Lerp(a.z, b.z, t)
    };
}


inline auto Exp(Vector3 const& v) -> Vector3
{
    return {
        std::expf(v.x),
        std::expf(v.y),
        std::expf(v.z)
    };
}

inline auto Dot(Vector3 const& a, Vector3 const& b) -> float
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline auto AngleCos(Vector3 const& a, Vector3 const b) -> float
{
    auto cos = Dot(a, b) / a.Length() / b.Length();
    if(cos > 1.0f)
    {
        cos = 1.0f;
    }
    return cos;
}


inline auto RayCircleIntersection(Vector3 const& rayOrigin, Vector3 const& rayDir, float const circleRadius) -> std::optional<Vector3>
{
    auto const r2 = circleRadius * circleRadius;

    auto const a = Dot(rayDir, rayDir);
    auto const b = 2 * Dot(rayOrigin, rayDir);
    auto const c = Dot(rayOrigin, rayOrigin) - r2;

    auto const d = b * b - 4 * a * c;


    if(d >= 0.0f)
    {
        auto const d2 = std::sqrtf(d);
        auto const t1 = (-b - d2) / 2.0f / a;

        if(t1 >= 0.0f)
        {
            return { rayOrigin + rayDir * t1 };
        }

        auto const t2 = (-b + d2) / 2.0f / a;

        if(t2 >= 0.0f)
        {
            return { rayOrigin + rayDir * t2 };
        }
    }

    return std::nullopt;
}