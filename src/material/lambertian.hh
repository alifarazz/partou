#pragma once
#include "../random/random.hh"
#include "../shapes/hitable.hh"  // for hit_info definition
#include "material.hh"

namespace partou
{
class Lambertian : public Material
{
public:
  Lambertian(const Spectrum& albedo)
      : m_albedo {albedo}
  {
  }

  auto scatter(const Ray&,
               const hit_info& info,
               Spectrum& attenuation,
               Ray& r_scattered) const -> bool final override
  { // doesn't depend on viewing direction (which is wi (which is r_in.dir() (r_in is the first arg)))
    auto scatter_direction = info.normal + random::unit_vec3<math::Float>();
    if (math::near_zero(scatter_direction))  // degenrate scatter direction
      scatter_direction = info.normal;
    r_scattered = Ray(info.p, scatter_direction);
    attenuation = m_albedo;
    return true;
  }

  Spectrum m_albedo;
};
}  // namespace partou
