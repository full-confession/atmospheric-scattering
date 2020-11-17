#include "ScatteringMap.hpp"
#include "TransmittanceMap.hpp"
#include "IrradianceMap.hpp"
#include "TextureExport.hpp"

auto main() -> int
{
    Atmos::PlanetProperties pp;


    pp.SetPlanetRadius(3400.0f);
    pp.SetAtmosphereHeight(50.0f);
    pp.SetRayleightScaleHeight(11.1f);
    pp.SetMieScaleHeight(2.0f);
    pp.SetRayleightScaleHeight(11.0f);
    pp.SetRayleightScatteringCoef(Vector3(0.0331f, 0.0135f, 0.0058f));
    pp.SetRayleightExtinctionCoef(Vector3(0.0331f, 0.0135f, 0.0058f));


    std::cout << "Computing transmittance map" << std::endl;
    auto transmittanceMap = Atmos::TransmittanceMap(512, pp, { 512 });
    transmittanceMap.Compute();

    Atmos::ExportTexture::ExportTexturePPM(transmittanceMap.GetTexture(), "transmittance.ppm", 1.0f);
    Atmos::ExportTexture::ExportTextureBinary16(transmittanceMap.GetTexture(), "transmittance.bin");

    std::cout << "Computing scattering map" << std::endl;
    auto scatteringMap = Atmos::ScatteringMap(512, 512, pp, { 256 }, { 256 });
    scatteringMap.Compute(
        Atmos::ScatteringMap::Mapping::Linear,
        Atmos::ScatteringMap::Mapping::Linear
    );

    Atmos::ExportTexture::ExportTexturePPM(scatteringMap.GetTexture(), "scattering.ppm");
    Atmos::ExportTexture::ExportTextureBinary16(scatteringMap.GetTexture(), "scattering.bin");


    std::cout << "Computing irradiance map" << std::endl;
    auto irradianceMap = Atmos::IrradianceMap(512, 128, pp, { 128 }, { 128 });
    irradianceMap.Compute();

    Atmos::ExportTexture::ExportTexturePPM(irradianceMap.GetTexture(), "irradiance.ppm", 10.0f);
    Atmos::ExportTexture::ExportTextureBinary16(irradianceMap.GetTexture(), "irradiance.bin");

    return 0;
}
