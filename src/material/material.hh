#pragma once

#include "../color/color.hh"
#include "../ray/ray.hh"

namespace partou
{
struct hit_info;

class Material
{
public:
  virtual auto scatter(const Ray& r_in,
                       const hit_info& info,
                       Spectrum& attenuation,
                       Ray& r_scattered) const -> bool = 0;

  virtual auto emitted(const math::Vec2f&, const math::Point3f&) const -> Spectrum
  {  // uv, p
    return Spectrum{0};
  }
};
}  // namespace partou
