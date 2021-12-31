#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Triangle : public Hitable
{
public:
  Triangle() = default; // should never be used actually
  Triangle(const math::Vec3f& v0,
           const math::Vec3f& v1,
           const math::Vec3f& v2,
           Material* matp)  // fix the material raw ptr
      ;
  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;
  auto computeBB() -> void;

  math::Vec3f v0, v1, v2;
  Material* mat_ptr;

protected:
};
}  // namespace partou
