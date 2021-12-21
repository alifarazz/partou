#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Triangle : public Hitable
{
public:
  Triangle() = default;
  Triangle(const Vec3f& v0,
           const Vec3f& v1,
           const Vec3f& v2,
           Material* matp)  // fix the material raw ptr
      : v0 {v0}
      , v1 {v1}
      , v2 {v2}
      , mat_ptr {matp}
  {
  }

  auto hit(const Ray& r, const Float t_min, const Float t_max, hit_info& info) const -> bool override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  Vec3f v0, v1, v2;
  Material* mat_ptr;
protected:
};
}  // namespace partou
