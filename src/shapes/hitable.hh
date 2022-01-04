#pragma once

#include <cmath>

#include "../math/general.hh"
#include "../math/vec.hh"
//
#include "../ray/ray.hh"
//
#include "../accel/AABB.hh"
//
#include "../math/transform.hh"

namespace partou
{
class Material;

struct hit_info
{
  bool is_front_facing;  // whether the ray is inside the object, or on the
                         // surface and outside of it
  int i;
  math::Float t;
  Material* mat_ptr;  /// fix me pls, I can be both a const and a ref
  math::Vec3f p;
  math::Vec3f normal;

  inline auto set_surface_normal(const Ray& r, const math::Vec3f& outward_surface_normal)
  {
    is_front_facing = outward_surface_normal.dot(-r.dir()) > 0;
    normal = is_front_facing ? outward_surface_normal : -outward_surface_normal;
  }
} __attribute__((aligned(64)));

class Hitable
{
public:
  accel::AABB aabb() const
  {
    return m_aabb;
  }

  virtual ~Hitable() {}
  virtual auto hit(const Ray& r,
                   const math::Float t_min,
                   const math::Float t_max,
                   hit_info& info) const -> bool = 0;
  virtual auto transformModel(const math::spatial::Transform& tModel) -> void = 0;

  // virtual bool bounding_box(math::Float t0, math::Float t1, AABB &box) const = 0;

  // AABB surrounding_box(const AABB& box0, const AABB& box1) const {
  //   auto fmin{AABB::ffmin}, fmax{AABB::ffmax};

  //   vec3 small{fmin(box0.min().x, box1.min().x),
  //              fmin(box0.min().y, box1.min().y),
  //              fmin(box0.min().z, box1.min().z)},
  //        big  {fmax(box0.max().x, box1.max().x),
  //              fmax(box0.max().y, box1.max().y),
  //              fmax(box0.max().z, box1.max().z)};

  //   return AABB(small, big);
  // }
protected:
  Hitable() = default;

  accel::AABB m_aabb;
};

}  // namespace partou
