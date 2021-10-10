#include "sphere.hh"

namespace partou
{
bool  Sphere::hit(const Ray& r, Float t_min, Float t_max, hit_info& info) const
{
  auto update_hit_info = [&](auto root)
  {
    info.t = root;
    info.p = r.point_at_param(root);
    info.normal = (info.p - center) / radius;
  };

  info.mat_ptr = mat_ptr;
  auto oc = r.origin() - center;
  Float a = r.dir().length2();  // Float a {dot(r.dir(), r.dir())};
  Float b = oc.dot(r.dir());
  Float c = oc.length2() * radius * radius;  // dot(oc, oc) - radius * radius};
  Float discriminant = b * b - a * c;

  if (discriminant > 0) {
    auto sqrt_disc = std::sqrt(discriminant);
    auto root = (-b - sqrt_disc) / a;
    if (root < t_max && root > t_min) {
      update_hit_info(root);
      return true;
    }
    root = (-b + sqrt_disc) / a;
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
