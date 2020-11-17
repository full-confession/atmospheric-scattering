#pragma once
#include "Vector3.hpp"
#include "Vector2.hpp"


namespace Atmos
{
    class PlanetProperties final
    {
    private:
        // Default Earth parameters
        float planetRadius = 6360.0f;
        float atmosphereHeight = 100.0f;

        Vector3 rayleightScatteringCoef = Vector3(0.0058f, 0.0135f, 0.0331f);
        Vector3 rayleightExtinctionCoef = Vector3(0.0058f, 0.0135f, 0.0331f);
    
        
        Vector3 mieScatteringCoef = Vector3(0.004f, 0.004f, 0.004f);
        Vector3 mieExtinctionCoef = Vector3(0.004f, 0.004f, 0.004f) / 0.9f;

        float rayleightScaleHeight = 8.0f;
        float mieScaleHeight = 1.2f;
        
        float miePhaseG = 0.8f;

    public:
        auto SetPlanetRadius(float const radius) -> void
        {
            planetRadius = radius;
        }

        [[nodiscard]]
        auto GetPlanetRadius() const -> float
        {
            return planetRadius;
        }

        auto SetAtmosphereHeight(float const height) -> void
        {
            atmosphereHeight = height;
        }
        
        [[nodiscard]]
        auto GetAtmosphereHeight() const -> float
        {
            return atmosphereHeight;
        }

        [[nodiscard]]
        auto GetAtmosphereRadius() const -> float
        {
            return planetRadius + atmosphereHeight;
        }

        auto SetRayleightScatteringCoef(Vector3 const& scatteringCoef) -> void
        {
            rayleightScatteringCoef = scatteringCoef;
        }
        
        [[nodiscard]]
        auto GetRayleightScatteringCoef() const -> Vector3 const&
        {
            return rayleightScatteringCoef;
        }

        auto SetRayleightExtinctionCoef(Vector3 const& extinctionCoef) -> void
        {
            rayleightExtinctionCoef = extinctionCoef;
        }
        
        [[nodiscard]]
        auto GetRayleightExtinctionCoef() const -> Vector3 const&
        {
            return rayleightExtinctionCoef;
        }


        auto SetMieScatteringCoef(Vector3 const& scatteringCoef) -> void
        {
            mieScatteringCoef = scatteringCoef;
        }

        [[nodiscard]]
        auto GetMieScatteringCoef() const -> Vector3 const&
        {
            return mieScatteringCoef;
        }

        auto SetMieExtinctionCoef(Vector3 const& extinctionCoef) -> void
        {
            mieExtinctionCoef = extinctionCoef;
        }

        [[nodiscard]]
        auto GetMieExtinctionCoef() const -> Vector3 const&
        {
            return mieExtinctionCoef;
        }


        auto SetRayleightScaleHeight(float const value) -> void
        {
            rayleightScaleHeight = value;
        }
        
        [[nodiscard]]
        auto GetRayleightScaleHeight() const -> float
        {
            return rayleightScaleHeight;
        }

        auto SetMieScaleHeight(float const value) -> void
        {
            mieScaleHeight = value;
        }

        [[nodiscard]]
        auto GetMieScaleHeight() const -> float
        {
            return mieScaleHeight;
        }

        auto SetMieAsymmetryCoef(float const value) -> void
        {
            miePhaseG = value;
        }
        
        [[nodiscard]]
        auto GetMieAsymmetryCoef() const -> float
        {
            return miePhaseG;
        }

        [[nodiscard]]
        auto RayleightDensityAltitude(float const altitude) const -> float
        {
            return std::expf(-altitude / rayleightScaleHeight);
        }

        [[nodiscard]]
        auto RayleightDensityRadius(float const radius) const -> float
        {
            return  RayleightDensityAltitude(radius - planetRadius);
        }


        [[nodiscard]]
        auto MieDensityAltitude(float const altitude) const -> float
        {
            return std::expf(-altitude / mieScaleHeight);
        }

        [[nodiscard]]
        auto MieDensityRadius(float const radius) const -> float
        {
            return MieDensityAltitude(radius - planetRadius);
        }


        [[nodiscard]]
        auto RayleightPhase(float const angle) const -> float
        {
            auto const cos = std::cosf(angle);
            return 3.0f / 16.0f / PI * (1.0f + cos * cos);
        }

        [[nodiscard]]
        auto RayleightPhaseCos(float const cos) const -> float
        {
            return 3.0f / 16.0f / PI * (1.0f + cos * cos);
        }

        [[nodiscard]]
        auto MiePhase(float const angle) const -> float
        {
            auto const g2 = miePhaseG * miePhaseG;
            auto const cos = std::cosf(angle);
            return 3.0f / 8.0f / PI * (1.0f - g2) * (1.0f + cos * cos) / (2.0f + g2) / std::powf(1.0f + g2 - 2.0f * miePhaseG * cos, 1.5f);
        }

        [[nodiscard]]
        auto MiePhaseCos(float const cos) const -> float
        {
            auto const g2 = miePhaseG * miePhaseG;
            return 3.0f / 8.0f / PI * (1.0f - g2) * (1.0f + cos * cos) / (2.0f + g2) / std::powf(1.0f + g2 - 2.0f * miePhaseG * cos, 1.5f);
        }
    };
}
