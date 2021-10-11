#pragma once

#include <cmath>

#include "../math/vec.hh"
// #include "AABB.hxx"
#include "../ray/ray.hh"

namespace partou
{
class Material;

struct hit_info
{
  bool is_front_facing;  // whether the ray is inside the object, or on the
                         // surface and outside of it
  Float t;
  Material* mat_ptr;  /// fix me pls, I can be both a const and a ref
  Vec3f p;
  Vec3f normal;

  inline auto set_surface_normal(const Ray& r,
                                 const Vec3f& outward_surface_normal)
  {
    is_front_facing = outward_surface_normal.dot(-r.dir()) > 0;
    normal = is_front_facing ? outward_surface_normal : -outward_surface_normal;
  }
};

class Hitable
{
public:
  virtual bool hit(const Ray& r,
                   float t_min,
                   float t_max,
                   hit_info& info) const = 0;

  // virtual bool bounding_box(float t0, float t1, AABB &box) const = 0;

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
};

}  // namespace partou
