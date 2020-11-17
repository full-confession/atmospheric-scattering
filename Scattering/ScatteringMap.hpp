#pragma once

#include "PlanetProperties.hpp"
#include "Texture.hpp"
#include "Transmittance.hpp"
#include "Scattering.hpp"

namespace Atmos
{
    class ScatteringMap final
    {
        PlanetProperties pp;
        Texture2D<Vector3> tex;

        Transmittance::IntegrationParameters tParams;
        Scattering::IntegrationParams sParams;

    public:
        explicit ScatteringMap(
            std::size_t const viewZenithCosResolution,
            std::size_t const sunZenithCosResolution,
            PlanetProperties const& planetProperties,
            Transmittance::IntegrationParameters const& tParams,
            Scattering::IntegrationParams const& sParams)
            : tex(viewZenithCosResolution, sunZenithCosResolution), pp(planetProperties), tParams(tParams), sParams(sParams)
        { }
        
        enum class Mapping
        {
            Linear,
            Cubic
        };

        
        auto Compute(
            Mapping const viewZenithMapping,
            Mapping const sunZenithMapping
        ) -> void
        {
            #pragma omp parallel for
            for(auto i = 0; i < static_cast<int>(tex.GetVResolution()); ++i)
            {
                auto const v = tex.IndexToV(i);
                auto const sunZenithCos = VToSunZenithCos(sunZenithMapping, v);
                
                for(auto j = 0; j < static_cast<int>(tex.GetUResolution()); ++j)
                {
                    auto const u = tex.IndexToU(j);
                    auto const viewZenithCos = UToViewZenithCos(viewZenithMapping, u);

                    tex[i][j] = Calculate(viewZenithCos, sunZenithCos);
                }
            }
        }

        auto GetTexture() const -> Texture2D<Vector3> const&
        {
            return tex;
        }


    private:

        [[nodiscard]]
        auto Calculate(float const viewZenithCos, float const sunZenithCos) const -> Vector3
        {
            auto const viewZenithSin = std::sin(std::acosf(viewZenithCos));
            auto const sunZenithSin = std::sin(std::acosf(sunZenithCos));
            
            auto const viewDir = Vector3(viewZenithSin, viewZenithCos, 0.0f);
            auto const sunDir = Vector3(
                sunZenithSin, 
                sunZenithCos, 
                0.0f
            );

            auto const viewPathEnterPoint = Vector3(0.0f, pp.GetPlanetRadius() + pp.GetAtmosphereHeight() * 0.95f, 0.0f);
            Vector3 viewPathExitPoint;

            auto const planetTest = RayCircleIntersection(viewPathEnterPoint, viewDir, pp.GetPlanetRadius());
            if(planetTest)
            {
                viewPathExitPoint = planetTest.value();
            }
            else
            {
                auto const atmoTest = RayCircleIntersection(viewPathEnterPoint, viewDir, pp.GetAtmosphereRadius());
                if(atmoTest)
                {
                    viewPathExitPoint = atmoTest.value();
                }
                else
                {
                    throw;
                }
            }

            return Scattering::GetPathScattering(viewPathEnterPoint, viewPathExitPoint, sunDir, pp, tParams, sParams);
        }

        auto static UToViewZenithCos(Mapping const mapping, float const u) -> float
        {
            // U				[0,  1]
            // ViewZenith		[0, -1]

            switch(mapping)
            {
            case Mapping::Linear:
                return -u;
            case Mapping::Cubic:
                return -std::powf(u, 3.0f);
            }
            return 0.0f;
        }

        auto static ViewZenithCosToU(Mapping const mapping, float const viewZenithCos) -> float
        {
            switch(mapping)
            {
            case Mapping::Linear:
                return -viewZenithCos;
            case Mapping::Cubic:
                return -std::powf(viewZenithCos, 1.0f / 3.0f);
            }
            return 0.0f;
        }

        auto static VToSunZenithCos(Mapping const mapping, float const v) -> float
        {
            // V				[0,  1]
            // SunZenith		[1, -1]

            switch(mapping)
            {
            case Mapping::Linear:
                return 1.0f - 2.0f * v;
            }
            return 0.0f;
        }

        auto static SunZenithCosToV(Mapping const mapping, float const viewZenithCos) -> float
        {
            switch(mapping)
            {
            case Mapping::Linear:
                return (1.0f - viewZenithCos) / 2.0f;
            }
            return 0.0f;
        }

        auto static WToSunAzimuthCos(float const v) -> float
        {
            // V				[0,  1]
            // SunAzimuth		[1, -1]
            return 1.0f - 2.0f * v;

        }

        auto static SunAzimuthCosToW(float const sunAzimuthCos) -> float
        {
            return (1.0f - sunAzimuthCos) / 2.0f;
        }
    };

}