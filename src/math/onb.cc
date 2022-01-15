#include "onb.hh"

namespace partou::math::spatial
{
ONB::ONB(const Vec3f& normal)
{
  this->n = normal.normalized(); // kinda redundant, but necessary in orginal defention of an ONB.
  const auto help_vec = std::abs(this->n.x) > .9 ? Vec3f {0, 1, 0} : Vec3f {1, 0, 0};
  this->t = this->n.cross(help_vec).normalize();
  this->s = this->n.cross(this->t);
}

auto ONB::local(Float a, Float b, Float c) const -> Vec3f
{
  return a * s + b * t + c * n;
}
auto ONB::local(const Vec3f& abc) const -> Vec3f
{
  return abc[0] * s + abc[1] * t + abc[2] * n;
}
}  // namespace partou::math::spatial
