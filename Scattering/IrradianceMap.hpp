#pragma once

#include <cmath>
#include "Vector2.hpp"
#include <random>
#include "Scattering.hpp"

namespace Atmos
{
    class IrradianceMap final
    {
        PlanetProperties pp;
        Texture1D<Vector3> tex;
        int semisphereSamples = 512;

        Transmittance::IntegrationParameters tParams;
        Scattering::IntegrationParams sParams;
        
    public:
        explicit IrradianceMap(
            std::size_t const resolution,
            int samples,
            PlanetProperties const& plantetProperties,
            Transmittance::IntegrationParameters const& tParams,
            Scattering::IntegrationParams const& sParams)
            : tex(resolution), semisphereSamples(samples), pp(plantetProperties), tParams(tParams), sParams(sParams)
        { }
        
        auto Compute() -> void
        {
            auto const dw = 2.0f * PI / static_cast<float>(semisphereSamples);
            auto directions = GenerateSemisphereDirections(semisphereSamples);

            #pragma omp parallel for
            for(auto i = 0; i < static_cast<int>(tex.GetUResolution()); ++i)
            {
                auto const u = tex.IndexToU(i);
                    
                auto const zenithCos = UToZenithCos(u);
                auto const zenithSin = std::sinf(std::acosf(zenithCos));
                auto const sunDir = Vector3(zenithSin, zenithCos, 0.0f);

                for(auto const& dir : directions)
                {
                    auto const light = Calculate(dir, sunDir);
                    tex[i] += light * dw;
                }
            }
        }

        [[nodiscard]]
        auto GetTexture() const -> Texture1D<Vector3> const&
        {
            return tex;
        }

    private:
        [[nodiscard]]
        auto GenerateSemisphereDirections(int const number) -> std::vector<Vector3>
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            auto result = std::vector<Vector3>();
            result.reserve(number);

            for(auto i = 0; i < number; ++i)
            {
                result.push_back(UniformSemispherePoint(gen));
            }

            return result;
        }

        [[nodiscard]]
        auto Calculate(Vector3 const& dir, Vector3 const& sunDir) const -> Vector3
        {
            auto const pathEnterPoint = Vector3(0.0f, pp.GetPlanetRadius() + pp.GetAtmosphereHeight() * 0.01f, 0.0f);
            auto const pathExitPoint = RayCircleIntersection(pathEnterPoint, dir, pp.GetAtmosphereRadius()).value();

            return Scattering::GetPathScattering(pathEnterPoint, pathExitPoint, sunDir, pp, tParams, sParams);
        }

        [[nodiscard]]
        auto static UToZenithCos(float const u) -> float
        {
            return 1.0f - 2.0f * u;
        }

        [[nodiscard]]
        auto static ZenithCosToU(float const zenithCos) -> float
        {
            return (1.0f - zenithCos) / 2.0f;
        }
        
        template <typename Engine>
        auto static UniformSemispherePoint(Engine& engine) -> Vector3
        {
            auto dis = std::uniform_real_distribution<float>(0.0f, 1.0f);
            
            auto azimuth = 2.0f * PI * dis(engine);
            auto zenith = std::asinf(std::sqrtf(dis(engine)));

            return {
                std::sin(zenith) * std::cos(azimuth),
                std::cos(zenith),
                std::sin(zenith) * std::sin(azimuth)
            };
        }
    };
}
