#pragma once

#include <cmath>
#include <memory>

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
  math::Float t;
  std::shared_ptr<Material> mat_ptr;  /// fix me pls, I can be both a const and a ref(?)
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

  virtual auto pdf_value(const math::Point3f&, const math::Vec3f&) const -> math::Float
      // {  // origin, dir
      //   exit(-2);
      //   return 0;
      // }
      = 0;
  virtual auto random(const math::Point3f&) const -> math::Vec3f
      // {  // origin
      //   exit(-2);
      //   return {1, 0, 0};
      // }
      = 0;

protected:
  Hitable() = default;

  accel::AABB m_aabb;
};

}  // namespace partou
