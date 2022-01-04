#include "sphere.hh"

#include "../accel/AABB.hh"
#include "../math/general.hh"

namespace partou
{
Sphere::Sphere(const math::Point3f& center, math::Float r, Material* matp)
    : center {center}
    , radius {r}
    , mat_ptr {matp}
{
  computeBoundingBox();
}

auto Sphere::computeBoundingBox() -> void
{
  const auto offset = decltype(this->center) {this->radius};
  this->m_aabb = accel::AABB(this->center - offset, this->center + offset);
}

auto Sphere::transformModel(const math::spatial::Transform& tModel) -> void
{
  center += tModel.m_translation;
  radius *= tModel.m_scale.x;  // not using y and z beacuse this is not an ellipsis
  // A sphere has not rotation around it's center, so tModel.m_rotation is inappropriate
  computeBoundingBox();
}

// clang-format off
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// # Solve for solutions of a 2nd degree equation.                                                          //
// > defenitions:                                                                                           //
//      p: any point, c: center of sphere, r: radius of sphere                                              //
// > sphere: (p - c) ** 2 = r ** 2                                                                          //
//   ray: p(t) = A + t * B, A -> origin of the ray, B -> direction of the ray                               //
// > mixing ray and sphere (t is the unknown variable):                                                     //
//     -> (p(t) - c) ** 2 = r ** 2                                                                          //
//     -> (A + t * B - c) ** 2 = r ** 2                                                                     //
//     -> (t * B) ** 2 + 2*t*dot(A - c, B) + (A - c) ** 2 - r ** 2 = 0                                      //
// Now, if there are two answers to this equation, it means the ray will intersect the sphere at two point. //
// If the equation has only one answer, the ray becomes tangent to the sphere.                              //
// No answer simply means no intersection.                                                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on
auto Sphere::hit(const Ray& r,
                 const math::Float t_min,
                 const math::Float t_max,
                 hit_info& info) const -> bool
{
  auto update_hit_info = [&](auto t)
  {
    info.t = t;  // t_min <= t < t_max
    info.p = r.eval_at(t);  // p is on the surface of the sphere
    auto surface_normal = ((info.p - this->center) / this->radius);  //.normalize();  // fix me!!!
    info.set_surface_normal(r, surface_normal);
    // if (i++ == 10) {
    //   std::cerr << "center:  " << this->center << "\n";
    //   std::cerr << "point:   " << info.p << std::endl;
    //   std::cerr << "radious: " << this->radius << std::endl;
    //   std::cerr << "r.origin(): " << r.orig() << std::endl;
    //   std::cerr << "r.dir():    " << r.dir() << std::endl;
    //   std::cerr << "r.o - s.center:  " << r.orig() - this->center << std::endl;
    //   std::cerr << "p - center:\t" << (this->center - info.p).length()
    //             << std::endl;
    //   std::cerr << "p**2:\t" << (info.p).length() << std::endl;
    //   exit(-1);
    // }
  };

  info.mat_ptr = mat_ptr;
  auto oc = r.orig() - this->center;  // A - c
  math::Float a = r.dir().length2();
  math::Float half_b = oc.dot(r.dir());
  math::Float c = oc.length2() - math::pow2(this->radius);
  math::Float delta_2 = half_b * half_b - a * c;

  // We discard the case with single answer because it's numerically unstable to
  // calculate it using floating point numbers. Plus, we need to find the
  // nearset root, i.e. the one with the least t.
  if (delta_2 > 0) {
    auto delta = std::sqrt(delta_2);
    auto t1 = (-half_b - delta) / a;
    if (t1 < t_max && t1 >= t_min) {
      update_hit_info(t1);
      return true;
    }
    auto t2 = (-half_b + delta) / a;
    if (t2 < t_max && t2 >= t_min) {
      update_hit_info(t2);
      return true;
    }
  }
  return false;
}

// bool Sphere::bounding_box(math::Float t0, math::Float t1, AABB& box) const
// {
//   vec3 r {radius, radius, radius};
//   box = AABB(center - r, center + r);
//   return true;
// };
}  // namespace partou
