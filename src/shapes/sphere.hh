#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Sphere : public Hitable
{
public:
  Sphere() = delete;
  Sphere(const math::Point3f& center,
         math::Float r,
         std::shared_ptr<Material> matp);  // fix the material raw ptr

  auto hit(const Ray& r, math::Float t_min, math::Float t_max, hit_info& info) const
      -> bool final override;
  auto transformModel(const math::spatial::Transform& tModel) -> void final override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  math::Point3f center;
  math::Float radius;
  std::shared_ptr<Material> mat_ptr;

protected:
  auto computeBoundingBox() -> void;
};
}  // namespace partou
