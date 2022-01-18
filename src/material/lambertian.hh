#pragma once

#include "../math/onb.hh"
#include "../random/random.hh"
#include "../sampling/pdf.hh"
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

  auto scatter(const Ray&, const hit_info& h_info, scatter_info& s_info) const
      -> bool final override
  {  // doesn't depend on viewing direction (which is wi (which is r_in.dir() (r_in is the first
     // arg)))
    s_info.is_specular = false;
    s_info.attenuation = m_albedo;
    s_info.pdf_ptr = std::move(std::make_shared<sampling::CosinePDF>(h_info.normal));
    return true;
  }

  auto scattering_pdf(const Ray&, const hit_info& hinfo, const Ray& scattered) const
      -> math::Float final override
  {  // s(dir) = cos theta / pi
    const auto r_scatterd_dir_normalized = scattered.dir().normalized();
    const auto cosine = hinfo.normal.dot(r_scatterd_dir_normalized);
    return std::max(cosine / math::PI, math::Float(0));
  }

  Spectrum m_albedo;
};
}  // namespace partou
