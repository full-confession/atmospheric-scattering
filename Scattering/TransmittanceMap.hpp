#pragma once
#include "Transmittance.hpp"
#include "Texture.hpp"

namespace Atmos
{

    class TransmittanceMap final
    {
        PlanetProperties pp;
        Texture1D<Vector3> tex;
        Transmittance::IntegrationParameters params;
        
    public:
        explicit TransmittanceMap(
            size_t const resolution,
            PlanetProperties const& plantetProperties,
            Transmittance::IntegrationParameters const& params)
            : pp(plantetProperties), tex(resolution), params(params)
        { }

        auto Compute() -> void
        {
            #pragma omp parallel for
            for(auto i = 0; i < static_cast<int>(tex.GetUResolution()); ++i)
            {
                auto const u = tex.IndexToU(i);
                auto const zenithCos = UToZenithCos(u);

                tex[i] = CalculateUsingZenithCos(zenithCos);
            }
        }
        
        [[nodiscard]]
        auto GetTexture() const -> Texture1D<Vector3> const&
        {
            return tex;
        }
        
    private:
        [[nodiscard]]
        auto CalculateUsingZenithCos(float const zenithCos) const -> Vector3
        {
            auto const zenithSin = std::sinf(std::acosf(zenithCos));
            auto const dir = Vector3(zenithSin, zenithCos, 0.0f);

            
            return CalculateUsingDirection(dir);
        }
        
        [[nodiscard]]
        auto CalculateUsingDirection(Vector3 const& dir) const -> Vector3
        {
            auto const pathEnterPoint = Vector3(0.0f, pp.GetPlanetRadius() + pp.GetAtmosphereHeight() * 0.01f, 0.0f);
            
            Vector3 pathExitPoint;

            auto const planetTest = RayCircleIntersection(pathEnterPoint, dir, pp.GetPlanetRadius());
            if(planetTest)
            {
                pathExitPoint = planetTest.value();
            }
            else
            {
                auto const atmoTest = RayCircleIntersection(pathEnterPoint, dir, pp.GetAtmosphereRadius());
                if(atmoTest)
                {
                    pathExitPoint = atmoTest.value();
                }
                else
                {
                    throw;
                }
            }

            return Transmittance::GetPathTransmittance(pathEnterPoint, pathExitPoint, pp, params);
        }

        [[nodiscard]]
        auto static UToZenithCos(float const u) -> float
        {
            return u;
        }
        
        [[nodiscard]]
        auto static ZenithCosToU(float const zenithCos) -> float
        {
            return zenithCos;
        }
    };

}