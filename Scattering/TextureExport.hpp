#pragma once
#include "Texture.hpp"
#include <fstream>
#include "Vector3.hpp"
#include "DirectXPackedVector.h"

namespace Atmos
{

    class ExportTexture final
    {
    public:
        static auto ExportTexturePPM(Texture2D<Vector3> const& texture, char const* const fileName) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            fout << "P6\n";
            fout << texture.GetUResolution() << " " << texture.GetVResolution() << "\n255\n";

            for(size_t i = 0; i < texture.GetVResolution(); ++i)
            {
                for(size_t j = 0; j < texture.GetUResolution(); ++j)
                {
                    uint8_t color[3];
                    color[0] = std::min(static_cast<int>(texture[i][j].x * 255 * 10), 255);
                    color[1] = std::min(static_cast<int>(texture[i][j].y * 255 * 10), 255);
                    color[2] = std::min(static_cast<int>(texture[i][j].z * 255 * 10), 255);

                    fout.write(reinterpret_cast<char*>(color), std::size(color));
                }
            }

            fout.close();
        }

        static auto ExportTexturePPM(Texture2D<unsigned char> const& texture, char const* const fileName) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            fout << "P6\n";
            fout << texture.GetUResolution() << " " << texture.GetVResolution() << "\n255\n";

            for(size_t i = 0; i < texture.GetVResolution(); ++i)
            {
                for(size_t j = 0; j < texture.GetUResolution(); ++j)
                {
                    uint8_t color[3];
                    color[0] = texture[i][j];
                    color[1] = texture[i][j];
                    color[2] = texture[i][j];

                    fout.write(reinterpret_cast<char*>(color), std::size(color));
                }
            }

            fout.close();
        }

        static auto ExportTexturePPM(Texture1D<Vector3> const& texture, char const* const fileName, float const multiplier) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            fout << "P6\n";
            fout << texture.GetUResolution() << " 1\n255\n";

            for(size_t i = 0; i < texture.GetUResolution(); ++i)
            {
                uint8_t color[3];
                color[0] = std::min(static_cast<int>(texture[i].x * multiplier * 255), 255);
                color[1] = std::min(static_cast<int>(texture[i].y * multiplier * 255), 255);
                color[2] = std::min(static_cast<int>(texture[i].z * multiplier * 255), 255);

                fout.write(reinterpret_cast<char*>(color), std::size(color));
            }

            fout.close();
        }

        static auto ExportTextureBinary16(Texture2D<Vector3> const& texture, char const* const fileName) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            struct Header final
            {
                uint16_t width;
                uint16_t height;
                uint16_t depth;
            } const header = {
                static_cast<uint16_t>(texture.GetUResolution()),
                static_cast<uint16_t>(texture.GetVResolution()),
                static_cast<uint16_t>(1)
            };

            fout.write(reinterpret_cast<char const*>(&header), sizeof header);
    
            for(size_t i = 0; i < texture.GetVResolution(); ++i)
            {
                for(size_t j = 0; j < texture.GetUResolution(); ++j)
                {
                    auto const color = texture[i][j];
                    auto const halfColor = DirectX::PackedVector::XMHALF4(color.x, color.y, color.z, 0.0f);

                    fout.write(reinterpret_cast<char const*>(&halfColor), sizeof halfColor);
                }
            }
        }

        static auto ExportTextureBinary16(Texture1D<Vector3> const& texture, char const* const fileName) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            struct Header final
            {
                uint16_t width;
                uint16_t height;
                uint16_t depth;
            } const header = {
                static_cast<uint16_t>(texture.GetUResolution()),
                static_cast<uint16_t>(1),
                static_cast<uint16_t>(1)
            };

            fout.write(reinterpret_cast<char const*>(&header), sizeof header);

            for(size_t i = 0; i < texture.GetUResolution(); ++i)
            {
                auto const color = texture[i];
                auto const halfColor = DirectX::PackedVector::XMHALF4(color.x, color.y, color.z, 0.0f);

                fout.write(reinterpret_cast<char const*>(&halfColor), sizeof halfColor);
            }
    
        }

        static auto ExportTextureBinary16(Texture3D<Vector3> const& texture, char const* const fileName) -> void
        {
            auto fout = std::ofstream(fileName, std::ios::out | std::ios::binary);
            if(!fout)
            {
                throw;
            }

            struct Header final
            {
                uint16_t width;
                uint16_t height;
                uint16_t depth;
            } const header = {
                static_cast<uint16_t>(texture.GetUResolution()),
                static_cast<uint16_t>(texture.GetVResolution()),
                static_cast<uint16_t>(texture.GetWResolution())
            };

            fout.write(reinterpret_cast<char const*>(&header), sizeof header);

            for(size_t k = 0; k < texture.GetVResolution(); ++k)
            {
                for(size_t i = 0; i < texture.GetVResolution(); ++i)
                {
                    for(size_t j = 0; j < texture.GetUResolution(); ++j)
                    {
                        auto const color = texture[k][i][j];
                        auto const halfColor = DirectX::PackedVector::XMHALF4(color.x, color.y, color.z, 0.0f);

                        fout.write(reinterpret_cast<char const*>(&halfColor), sizeof halfColor);
                    }
                }
            }
        }

    };
}