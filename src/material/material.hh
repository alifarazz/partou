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
                       Ray& r_scattered,
                       math::Float& pdf) const -> bool
  {
    return false;
  }

  virtual auto scattering_pdf(const Ray& r_in, const hit_info& hinfo, const Ray& scattered) const
      -> math::Float
  {
    return 0;
  }

  virtual auto emitted(const math::Vec2f&, const hit_info&, const math::Point3f&) const -> Spectrum
  {  // uv, hinfo, p
    return Spectrum {0};
  }
};
}  // namespace partou
