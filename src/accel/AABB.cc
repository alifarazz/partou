#include "AABB.hh"

#include "../perf_stats/stats.hh"

namespace partou::accel
{
bool AABB::intersect(const Ray& r, math::Float& t) const
{
  partou::stats::numRayBBoxTests++;

  t = 0;
  auto t_max = std::numeric_limits<math::Float>::max();
  for (int i = 0; i < 3; i++) {
    auto d_inv = 1 / r.dir()[i];

    auto t0 = (m_min[i] - r.orig()[i]) * d_inv;
    auto t1 = (m_max[i] - r.orig()[i]) * d_inv;
    if (d_inv < 0)
      std::swap(t0, t1);

    if (t < t0)
      t = t0;
    if (t1 < t_max)
      t_max = t1;
    if (t_max < t)
      return false;
  }
  return true;
}

bool AABB::contains(const math::Point3f& p) const
{
  return p.x >= m_min.x && p.y >= m_min.y && p.z >= m_min.z && p.x <= m_max.x && p.y <= m_max.y
      && p.z <= m_max.z;
}

math::Vec3f AABB::basis() const
{
  return m_max - m_min;
}

math::Point3f AABB::centroid() const
{
  return (m_max + m_min) / 2;
}

math::Float AABB::area() const
{
  if (!this->valid())
    return 0;
  auto d = this->basis();
  return 2 * (d.x * d.y + d.x * d.z + d.z * d.y);
}

void AABB::merge(const AABB& BB)
{
  for (int i = 0; i < 3; i++) {
    if (m_min[i] > BB.m_min[i])
      m_min[i] = BB.m_min[i];
    if (m_max[i] < BB.m_max[i])
      m_max[i] = BB.m_max[i];
  }
}

void AABB::merge(const math::Point3f& p)
{
  for (int i = 0; i < 3; i++) {
    if (m_min[i] > p[i])
      m_min[i] = p[i];
    if (m_max[i] < p[i])
      m_max[i] = p[i];
  }
}

bool AABB::valid() const
{
  for (int i = 0; i < 3; i++) {
    if (m_min[i] > m_max[i])
      return false;
  }
  return true;
}
}  // namespace partou::accel
