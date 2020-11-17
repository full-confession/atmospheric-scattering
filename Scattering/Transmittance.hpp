#pragma once

#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Texture.hpp"
#include "PlanetProperties.hpp"

namespace Atmos
{
    class Transmittance  final
    {
    public:
        struct IntegrationParameters final
        {
            int sampleCount = 512;
        };

        [[nodiscard]]
        static auto GetPathTransmittance(
            Vector3 const& a,
            Vector3 const& b,
            PlanetProperties const& pp,
            IntegrationParameters const& params) -> Vector3
        {
            auto const path = b - a;
            auto const pathDelta = path / static_cast<float>(params.sampleCount);
            auto const pathDeltaLength = pathDelta.Length();

            auto rayleightPathDensity = 0.0f;
            auto miePathDensity = 0.0f;

            auto const firstPoint = a + pathDelta / 2.0f;
            for(auto i = 0; i < params.sampleCount; ++i)
            {
                auto const pathPoint = firstPoint + pathDelta * static_cast<float>(i);
                auto const pathPointRadius = pathPoint.Length();


                rayleightPathDensity += pp.RayleightDensityRadius(pathPointRadius);
                miePathDensity += pp.MieDensityRadius(pathPointRadius);
            }

            auto const pathOpticalDepth = rayleightPathDensity * pp.GetRayleightExtinctionCoef()
                + miePathDensity * pp.GetMieExtinctionCoef();

            return Exp(-pathOpticalDepth * pathDeltaLength);
        }
    };
}
