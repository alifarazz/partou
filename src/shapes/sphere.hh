#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Sphere : public Hitable
{
public:
  Sphere() {}
  Sphere(Vec3f center, Float r, Material* matp)  // fix the material raw ptr
      : center {center}
      , radius {r}
      , mat_ptr {matp}
  {
  }

  bool hit(const Ray& r,
           Float t_min,
           Float t_max,
           hit_info& info) const override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  Vec3f center;
  Float radius;
  Material* mat_ptr;
};
}  // namespace partou
