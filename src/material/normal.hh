#pragma once
#include "../random/random.hh"
#include "../shapes/hitable.hh"  // for hit_info definition
#include "material.hh"

namespace partou
{
class Normal : public Material
{
public:
  Normal() = default;
  auto scatter(const Ray& r_in,
               const hit_info& info,
               Spectrum& attenuation,
               Ray& r_scattered) const -> bool final override
  { // FIXME: currently borken, should find a way to stop scattering of ray after hitting normal material
    attenuation = (info.normal + Spectrum(1.0F)) / 2.0F;
    return false;
  }

  Spectrum m_albedo;
};
}  // namespace partou
