#include "triangle.hh"

#include "../perf_stats/stats.hh"

namespace partou
{

Triangle::Triangle(const math::Vec3f& v0,
                   const math::Vec3f& v1,
                   const math::Vec3f& v2,
                   std::shared_ptr<Material> matp)
    : v0 {v0}
    , v1 {v1}
    , v2 {v2}
    , mat_ptr {matp}
{
  this->precomputeValues();  // init E01_ and E02_
  vn0 = vn1 = vn2 = E01_.cross(E02_).normalize();  // for flat shading
  this->computeBoundingBox();
}

Triangle::Triangle(const math::Vec3f& v0,
                   const math::Vec3f& v1,
                   const math::Vec3f& v2,
                   const math::Vec3f& vn0,
                   const math::Vec3f& vn1,
                   const math::Vec3f& vn2,
                   std::shared_ptr<Material> matp)
    : v0 {v0}
    , v1 {v1}
    , v2 {v2}
    , vn0 {vn0}
    , vn1 {vn1}
    , vn2 {vn2}
    , mat_ptr {matp}
{
  this->precomputeValues();
  this->computeBoundingBox();
}

auto Triangle::precomputeValues() -> void
{
  E01_ = v1 - v0;
  E02_ = v2 - v0;
}

auto Triangle::computeBoundingBox() -> void
{
  this->m_aabb = accel::AABB();
  for (const auto& v : {v0, v1, v2})
    (this->m_aabb).merge(v);
}

auto Triangle::interpolatedNormal(const math::Vec2f& st) const -> math::Vec3f
{
  const auto interpolated = (1 - st[0] - st[1]) * vn0 + st[0] * vn1 + st[1] * vn2;
  return interpolated.normalized();
}

auto Triangle::transformModel(const math::spatial::Transform& tModel) -> void  // virtual function
{
  v0 = tModel.transformPoint(v0);
  v1 = tModel.transformPoint(v1);
  v2 = tModel.transformPoint(v2);

  vn0 = tModel.transformNormal(vn0);
  vn1 = tModel.transformNormal(vn1);
  vn2 = tModel.transformNormal(vn2);

  this->precomputeValues();
  this->computeBoundingBox();
}

// from:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/raytri_tam.pdf
auto Triangle::hit(const Ray& r,
                   const math::Float t_min,
                   const math::Float t_max,
                   hit_info& info) const -> bool  // virtual function
{
  partou::stats::numRayTrianglesTests++;

  const auto pv = r.dir().cross(E02_);  // perpendicular vector
  const auto det = E01_.dot(pv);

  constexpr auto kEpsilon = 100 * std::numeric_limits<decltype(det)>::epsilon();
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

  const auto qv = tv.cross(E01_);
  const auto v = qv.dot(r.dir()) * inv_det;  // 2nd barycentric coordinate
  if (1 < v || v < 0 || 1 < u + v)  // if ray misses the triangle
    return false;

  const auto t = qv.dot(E02_) * inv_det;
  if (t < t_min || t_max < t)  // are we in render ?
    return false;

  info.t = t;
  info.p = r.eval_at(t);
  const auto normal = interpolatedNormal({u, v});
  // const auto normal = E01_.cross(E02_).normalize();
  info.set_surface_normal(r, normal);
  info.mat_ptr = this->mat_ptr;

  partou::stats::numRayTrianglesIsect++;

  return true;
}
}  // namespace partou
