#pragma once

#include <cmath>

#include "hitable.hh"
// #include "material.hxx"

namespace partou
{
class Triangle : public Hitable
{
public:
  Triangle() = default;  // should never be used actually
  Triangle(const math::Vec3f& v0,
           const math::Vec3f& v1,
           const math::Vec3f& v2,
           std::shared_ptr<Material> matp = nullptr);
  Triangle(const math::Vec3f& v0,
           const math::Vec3f& v1,
           const math::Vec3f& v2,
           const math::Vec3f& vn0,
           const math::Vec3f& vn1,
           const math::Vec3f& vn2,
           std::shared_ptr<Material> matp = nullptr);

  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool final override;
  auto transformModel(const math::spatial::Transform& tModel) -> void final override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  auto precomputeValues() -> void;
  auto computeBoundingBox() -> void;
  auto interpolatedNormal(const math::Vec2f& st) const -> math::Vec3f;

protected:
  // underscore at the end of the name means it'll be precomputed (for example, in the constructor)
  math::Vec3f v0, v1, v2;
  math::Vec3f E01_, E02_;  // precomputed barycentric axis
  // math::Vec3f normal_;  // precomputed normal of the plane in which the triangle resides
  math::Vec3f vn0, vn1, vn2;

  std::shared_ptr<Material> mat_ptr;
};
}  // namespace partou
