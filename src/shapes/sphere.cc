#include "sphere.hh"

#include "../math/general.hh"

namespace partou
{
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

bool Sphere::hit(const Ray& r, Float t_min, Float t_max, hit_info& info) const
{
  auto update_hit_info = [&](auto root)
  {
    info.t = root;
    info.p = r.eval_at(root);
    info.normal = (info.p - center) / radius;
  };

  info.mat_ptr = mat_ptr;
  auto oc = r.orig() - center; // A - c
  Float a = r.dir().length2();
  Float b = oc.dot(r.dir());
  Float c = oc.length2() * math::pow2(radius);
  Float delta_2 = b * b - a * c;

  // We discard the one answer case because it's numerically unstable to
  // calculate it using floating point numbers.
  if (delta_2 > 0) {
    auto delta = std::sqrt(delta_2);
    auto root = (-b - delta) / a;
    if (root < t_max && root > t_min) {
      update_hit_info(root);
      return true;
    }
    root = (-b + delta) / a;
    if (root < t_max && root > t_min) {
      update_hit_info(root);
      return true;
    }
  }
  return false;
}

// bool Sphere::bounding_box(Float t0, Float t1, AABB& box) const
// {
//   vec3 r {radius, radius, radius};
//   box = AABB(center - r, center + r);
//   return true;
// };
}  // namespace partou
