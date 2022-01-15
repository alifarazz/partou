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
               Spectrum& albedo,
               Ray& r_scattered,
               math::Float& pdf) const -> bool final override
  {  // doesn't depend on viewing direction (which is wi (which is r_in.dir() (r_in is the first
     // arg)))
     // auto scatter_direction = info.normal + random::unit_vec3<math::Float>();
     // if (math::near_zero(scatter_direction))  // degenrate scatter direction
     //   scatter_direction = info.normal;
    const auto scatter_direction = random::vec_in_hemisphere(info.normal);
    r_scattered = Ray(info.p, scatter_direction);
    albedo = m_albedo;
    pdf = info.normal.dot(r_scattered.dir().normalized()) / math::PI;
    return true;
  }

  auto scattering_pdf(const Ray& r_in, const hit_info& hinfo, const Ray& scattered) const
      -> math::Float final override
  { // s(dir) = cos theta / pi
    const auto r_scatterd_dir_normalized = scattered.dir().normalized();
    const auto cosine = hinfo.normal.dot(r_scatterd_dir_normalized);
    return std::max(cosine / math::PI, math::Float(0));
  }

  Spectrum m_albedo;
};
}  // namespace partou
