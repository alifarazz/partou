#pragma once

#include <limits>

#include "../math/vec.hh"
#include "../ray/ray.hh"

namespace partou::accel
{
struct AABB
{
  AABB() = default;
  AABB(const math::Point3f& minimum, const math::Point3f& maximum)
      : m_min {minimum}
      , m_max {maximum}
  {
  }

  bool intersect(const Ray& r, math::Float& t) const;
  bool contains(const math::Point3f& p) const;
  math::Vec3f basis() const;
  math::Point3f centroid() const;
  math::Float area() const;
  // distance2
  // max_distance2
  void merge(const AABB& BB);
  void merge(const math::Point3f& p);
  bool valid() const;

  math::Point3f m_min {std::numeric_limits<math::Float>::max()};
  math::Point3f m_max {std::numeric_limits<math::Float>::lowest()};
};
}  // namespace partou::accel
