#pragma once
#include "../../random/random.hh"
#include "../../shapes/hitable.hh"  // for hit_info definition
#include "../material.hh"

namespace partou
{
class DiffuseLight : public Material
{
public:
  DiffuseLight(const Spectrum& albedo)
      : m_albedo {albedo}
  {
  }

  auto scatter(const Ray&, const hit_info&, Spectrum&, Ray&, math::Float&) const
      -> bool final override
  {
    return false;
  }

  auto emitted(const math::Vec2f&, const hit_info& hinfo, const math::Point3f&) const
      -> Spectrum final override
  {  // uv, hinfo, p
    if (hinfo.is_front_facing)
      return m_albedo;
    return Spectrum {0};
  }

  Spectrum m_albedo;
};
}  // namespace partou
