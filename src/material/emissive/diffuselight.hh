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

  auto scatter(const Ray&, const hit_info&, Spectrum&, Ray&) const -> bool final override
  {
    return false;
  }

  auto emitted(const math::Vec2f&, const math::Point3f&) const -> Spectrum final override
  {  // uv, p
    return m_albedo;
  }

  Spectrum m_albedo;
};
}  // namespace partou
