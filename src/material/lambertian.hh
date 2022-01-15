#pragma once
#include "../math/onb.hh"
#include "../random/random.hh"
#include "../shapes/hitable.hh"  // for hit_info definition
#include "material.hh"

namespace partou
{
class Lambertian : public Material
{
public:
  static inline math::Vec3f random_cosine_direction()
  {
    using namespace partou::random;
    using namespace partou::math;

    const auto r1 = unit<Float>(), r2 = unit<Float>();
    const auto z = std::sqrt(1 - r2);

    const auto phi = 2 * PI * r1;
    const auto x = std::cos(phi) * std::sqrt(r2);
    const auto y = std::sin(phi) * std::sqrt(r2);

    return {x, y, z};
  }

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
    const math::spatial::ONB onb {info.normal};
    const auto scatter_direction = onb.local(this->random_cosine_direction());
    r_scattered = Ray(info.p, scatter_direction.normalized());
    albedo = m_albedo;
    pdf = onb.n.dot(r_scattered.dir().normalized()) / math::PI;
    return true;
  }

  auto scattering_pdf(const Ray& r_in, const hit_info& hinfo, const Ray& scattered) const
      -> math::Float final override
  {  // s(dir) = cos theta / pi
    const auto r_scatterd_dir_normalized = scattered.dir().normalized();
    const auto cosine = hinfo.normal.dot(r_scatterd_dir_normalized);
    return std::max(cosine / math::PI, math::Float(0));
  }

  Spectrum m_albedo;
};
}  // namespace partou
