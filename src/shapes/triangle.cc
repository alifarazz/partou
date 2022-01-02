#include "triangle.hh"

#include "../perf_stats/stats.hh"

namespace partou
{

Triangle::Triangle(const math::Vec3f& v0,
                   const math::Vec3f& v1,
                   const math::Vec3f& v2,
                   Material* matp)
    : v0 {v0}
    , v1 {v1}
    , v2 {v2}
    , E01_ {v1 - v0}
    , E02_ {v2 - v0}
    , mat_ptr {matp}
{
  this->computeBB();
}

auto Triangle::computeBB() -> void
{
  this->m_aabb = accel::AABB();
  for (const auto& v : {v0, v1, v2})
    (this->m_aabb).merge(v);
}

// from:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/raytri_tam.pdf
auto Triangle::hit(const Ray& r,
                   const math::Float t_min,
                   const math::Float t_max,
                   hit_info& info) const -> bool
{
  partou::stats::numRayTrianglesTests++;
  const math::Vec3f E01 = v1 - v0;
  const math::Vec3f E02 = v2 - v0;

  const auto pv = r.dir().cross(E02_);  // perpendicular vector
  const auto det = E01_.dot(pv);

  constexpr auto kEpsilon = 100 * std::numeric_limits<decltype(det)>::epsilon();
  // If the determinant is negative, the triangle is backfacing. We want to render these ones as
  // well, or else they'll completely disappear from the scene.
  // If the determinant is close to 0, the ray is parallel to the triangle.
  if (det < 0 && det > -kEpsilon)
    return false;

  const auto inv_det = 1 / det;

  const auto tv = r.orig() - v0;
  const auto u = pv.dot(tv) * inv_det;  // 1st barycentric coordinate
  if (1 < u || u < 0)  // if ray misses the triangle
    return false;

  const auto qv = tv.cross(E01_);
  const auto v = qv.dot(r.dir()) * inv_det;  // 2nd barycentric coordinate
  if (1 < v || v < 0 || 1 < u + v)  // if ray misses the triangle
    return false;

  const auto t = qv.dot(E02_) * inv_det;
  if (t < t_min || t_max < t)  // are we in render ?
    return false;

  info.t = t;
  const auto normal = E01.cross(E02).normalize();
  info.set_surface_normal(r, normal);

  partou::stats::numRayTrianglesIsect++;

  return true;
}
}  // namespace partou
