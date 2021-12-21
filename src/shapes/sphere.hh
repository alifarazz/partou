#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Sphere : public Hitable
{
public:
  Sphere() = default;
  Sphere(const math::Vec3f& center, math::Float r, Material* matp)  // fix the material raw ptr
      : center {center}
      , radius {r}
      , mat_ptr {matp}
  {
  }

  auto hit(const Ray& r, math::Float t_min, math::Float t_max, hit_info& info) const
      -> bool override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  math::Vec3f center;
  math::Float radius;
  Material* mat_ptr;
};
}  // namespace partou
