#pragma once
#include <cmath>
#include <optional>
#include <iostream>

static inline constexpr float PI = 3.14159265358979323846f;

struct Vector2 final
{
    float x = 0.0f;
    float y = 0.0f;

    Vector2() = default;
    Vector2(float const x, float const y)
        : x(x), y(y)
    { }

    [[nodiscard]]
    auto Length() const -> float
    {
        return std::sqrtf(x * x + y * y);
    }

    auto operator+=(Vector2 const& v) -> Vector2&
    {
        x += v.x;
        y += v.y;
        return *this;
    }
};

inline auto operator-(Vector2 const& a, Vector2 const& b) -> Vector2
{
    return { a.x - b.x, a.y - b.y };
}

inline auto operator+(Vector2 const& a, Vector2 const& b) -> Vector2
{
    return { a.x + b.x, a.y + b.y };
}

inline auto operator*(Vector2 const& a, float const b) -> Vector2
{
    return { a.x * b, a.y * b};
}

inline auto operator/(Vector2 const& a, float const b) -> Vector2
{
    return { a.x / b, a.y / b};
}

inline auto Dot(Vector2 const& a, Vector2 const& b) -> float
{
    return a.x * b.x + a.y * b.y;
}

inline auto operator<<(std::ostream& os, Vector2 const& v) -> std::ostream &
{
    os << '(' << v.x << " ," << v.y << ')';
    return os;
}

inline auto RayCircleIntersection(Vector2 const& rayOrigin, Vector2 const& rayDir, float const circleRadius) -> std::optional<Vector2>
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