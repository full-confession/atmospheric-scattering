#pragma once

#include <vector>

namespace Atmos
{
    template <typename T>
    class Texture1D final
    {
    public:
        Texture1D()
            : uResolution(0), data()
        { }
    
        explicit Texture1D(std::size_t const uResolution)
            : uResolution(uResolution), data(uResolution)
        { }

        auto Sample(float const u) const -> T
        {
            auto const d = u * (uResolution - 1);
            auto const i0 = static_cast<std::size_t>(d);

            if(i0 == uResolution - 1)
            {
                return data[i0];
            }

            auto t = d - i0;


            return Lerp(data[i0], data[i0 + 1], t);
        }

        auto operator[](std::size_t const index) -> T&
        {
            return data[index];
        }

        auto operator[](std::size_t const index) const -> T const&
        {
            return data[index];
        }

        [[nodiscard]]
        auto IndexToU(std::size_t const index) const -> float
        {
            return static_cast<float>(index) / uResolution + 1.0f / (2.0f * static_cast<float>(uResolution));
        }
    
        [[nodiscard]]
        auto GetUResolution() const -> std::size_t
        {
            return uResolution;
        }

    private:
        std::size_t uResolution;
        std::vector<T> data;
    };

    template <typename T>
    class Texture2D final
    {
    public:
        Texture2D()
            : vResolution(0), data()
        { }
    
        Texture2D(std::size_t const uResolution, std::size_t const vResolution)
            : vResolution(vResolution), data(vResolution)
        {
            for(std::size_t i = 0; i < vResolution; ++i)
            {
                data[i] = Texture1D<T>(uResolution);
            }
        }

        auto Sample(float const u, float const v) const -> T
        {
            auto const d = v * (vResolution - 1);
            auto const i0 = static_cast<std::size_t>(d);

            if(i0 == vResolution - 1)
            {
                return data[i0].Sample(u);
            }

            auto t = d - i0;

            return Lerp(data[i0].Sample(u), data[i0 + 1].Sample(u), t);
        }

        auto operator[](std::size_t index) -> Texture1D<T>&
        {
            return data[index];
        }

        auto operator[](std::size_t index) const -> Texture1D<T> const&
        {
            return data[index];
        }
    
        [[nodiscard]]
        auto IndexToV(std::size_t const index) const -> float
        {
            return static_cast<float>(index) / vResolution + 1.0f / (2.0f * static_cast<float>(vResolution));
        }

        auto IndexToU(std::size_t const index) const -> float
        {
            return data[0].IndexToU(index);
        }


        [[nodiscard]]
        auto GetUResolution() const -> std::size_t
        {
            return data[0].GetUResolution();
        }
    
        [[nodiscard]]
        auto GetVResolution() const -> std::size_t
        {
            return vResolution;
        }

    private:
        std::size_t vResolution;
        std::vector<Texture1D<T>> data;
    };

    template <typename T>
    class Texture3D final
    {
    public:
        Texture3D()
            : wResolution(0), data()
        { }

        Texture3D(std::size_t const uResolution, std::size_t const vResolution, std::size_t const wResolution)
            : wResolution(wResolution), data(wResolution)
        {
            for(std::size_t i = 0; i < wResolution; ++i)
            {
                data[i] = Texture2D<Vector3>(uResolution, vResolution);
            }
        }

        auto Sample(float const u, float const v, float const w) const -> T
        {
            auto const d = w * (wResolution - 1);
            auto const i0 = static_cast<std::size_t>(d);

            if(i0 == wResolution - 1)
            {
                return data[i0].Sample(u, v);
            }

            auto t = d - i0;

            return Lerp(data[i0].Sample(u, v), data[i0 + 1].Sample(u, v), t);
        }

        auto operator[](std::size_t index) -> Texture2D<T> &
        {
            return data[index];
        }

        auto operator[](std::size_t index) const -> Texture2D<T> const&
        {
            return data[index];
        }

        [[nodiscard]]
        auto IndexToW(std::size_t const index) const -> float
        {
            return static_cast<float>(index) / wResolution + 1.0f / (2.0f * static_cast<float>(wResolution));
        }

        auto IndexToV(std::size_t const index) const -> float
        {
            return data[0].IndexToV(index);
        }

        auto IndexToU(std::size_t const index) const -> float
        {
            return data[0].IndexToU(index);
        }


        [[nodiscard]]
        auto GetUResolution() const -> std::size_t
        {
            return data[0].GetUResolution();
        }

        [[nodiscard]]
        auto GetVResolution() const -> std::size_t
        {
            return data[0].GetVResolution();
        }

        [[nodiscard]]
        auto GetWResolution() const -> std::size_t
        {
            return wResolution;
        }

    private:
        std::size_t wResolution;
        std::vector<Texture2D<T>> data;
    };
}