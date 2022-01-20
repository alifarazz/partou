#pragma once

#include "./utils.hh"
#include "material.hh"
//
#include "../random/random.hh"
#include "../shapes/hitable.hh"  // for hit_info definition

namespace partou
{
class Metal : public Material
{
public:
  Metal(const Spectrum& albedo, math::Float fuzziness = 0)
      : m_fuzz {std::min(fuzziness, math::Float(1))}
      , m_albedo {albedo}
  {
  }

  auto scatter(const Ray& r_in, const hit_info& h_info, scatter_info& s_info) const
      -> bool final override
  {
    const auto reflected = utils::reflect(r_in.dir().normalized(), h_info.normal);
    s_info.specular_ray = Ray(h_info.p, reflected + m_fuzz * random::vec_in_sphere<math::Float>());
    s_info.attenuation = m_albedo;
    s_info.is_specular = true;
    // s_info.pdf_ptr = nullptr; // HACK HACK HACK
    return true;
  }

  math::Float m_fuzz;  // NOTE: if I used microfacet models, this would be unnecessary.
  Spectrum m_albedo;  // NOTE: metal shouldn't have albedo, it should have a Complex IOR. From
                      // there, I should calculate the way metal scatters the incoming ray. But,
                      // it's not that simple and I'm not advanced enough.
};
}  // namespace partou
