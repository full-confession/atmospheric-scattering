#pragma once
#include "Texture.hpp"
#include "Vector3.hpp"
#include "PlanetProperties.hpp"
#include "Transmittance.hpp"

namespace Atmos
{

    class Scattering final
    {
    public:
        struct IntegrationParams final
        {
            int sampleCount = 512;
        };

        static auto GetPathScattering(
            Vector3 const& a,
            Vector3 const& b,
            Vector3 const& sunDir,
            PlanetProperties const& pp,
            Transmittance::IntegrationParameters const& tParams,
            IntegrationParams const& params) -> Vector3
        {
            auto const path = b - a;
            auto const pathDeltaVector = path / static_cast<float>(params.sampleCount);
            auto const pathDelta = pathDeltaVector.Length();
            auto const firstViewPathPoint = a + pathDeltaVector / 2.0f;

            auto const viewSunCos = AngleCos(path, sunDir);

            auto rayleightScattering = Vector3();
            auto mieScattering = Vector3();
            for(auto i = 0; i < params.sampleCount; ++i)
            {
                auto const viewPathPoint = firstViewPathPoint + pathDeltaVector * i;
                auto const transmittanceToViewEnterPoint = Transmittance::GetPathTransmittance(viewPathPoint,
                    a, pp, tParams);

                auto const sunBlockedByPlanet = RayCircleIntersection(viewPathPoint, sunDir, pp.GetPlanetRadius());
                if(sunBlockedByPlanet)
                {
                    continue;
                }
            
                auto const sunPathEnterPoint
                    = RayCircleIntersection(viewPathPoint, sunDir, pp.GetAtmosphereRadius()).value();

                auto const transmittanceToSunEnterPoint = Transmittance::GetPathTransmittance(viewPathPoint,
                    sunPathEnterPoint, pp, tParams);

                auto const lightPathTransmittance = transmittanceToSunEnterPoint * transmittanceToViewEnterPoint;
                auto const pointRadius = viewPathPoint.Length();
                
                rayleightScattering += lightPathTransmittance * pp.RayleightDensityRadius(pointRadius);
                mieScattering += lightPathTransmittance * pp.MieDensityRadius(pointRadius);
            }

            auto scattering = rayleightScattering * pp.RayleightPhaseCos(viewSunCos) * pp.GetRayleightScatteringCoef()
                + mieScattering * pp.MiePhaseCos(viewSunCos) * pp.GetMieScatteringCoef();
            scattering *= pathDelta;
            
            return scattering;
        }
    };
}