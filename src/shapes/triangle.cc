#include "triangle.hh"

namespace partou
{

// from:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
auto Triangle::hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const -> bool
{
  const math::Vec3f E01 = v1 - v0;
  const math::Vec3f E02 = v2 - v0;

  const auto pv = r.dir().cross(E02);  // perpendicular vector
  const  auto det = E01.dot(pv);

  constexpr auto kEpsilon = std::numeric_limits<decltype(det)>::epsilon();
  // If the determinant is negative, the triangle is backfacing. We want to render these ones as
  // well, or else they'll completely disappear from the scene.
  // If the determinant is close to 0, the ray is parallel to the triangle.
  if (det < kEpsilon && det > -kEpsilon)
    return false;

 const auto inv_det = 1 / det;

  const auto tv = r.orig() - v0;
  const auto u = pv.dot(tv) * inv_det;  // 1st barycentric coordinate
  if (1 < u || u < 0)  // if ray misses the triangle
    return false;

 const  auto qv = tv.cross(E01);
 const  auto v = qv.dot(r.dir()) * inv_det;  // 2nd barycentric coordinate
  if (1 < v || v < 0 || 1 < u + v)  // if ray misses the triangle
    return false;

 const auto t = qv.dot(E02) * inv_det;
  if (t < t_min || t_max < t)  // are we in render ?
    return false;

 const auto normal = E01.cross(E02).normalize();
  info.set_surface_normal(r, normal);
  return true;
}
}  // namespace partou