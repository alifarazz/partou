#pragma once

#include "../math/vec.hh"

namespace partou
{
class Ray
{
public:
  math::Point3f a;
  math::Vec3f b;

  Ray() = default;
  Ray(const math::Point3f& a, const math::Vec3f& b)
      : a {a}
      , b {b}
  {
  }

  [[nodiscard]] auto orig() const -> const decltype(a)&
  {
    return a;
  }
  [[nodiscard]] auto dir() const -> const decltype(b)&
  {
    return b;
  }
  [[nodiscard]] auto eval_at(math::Float t) const -> math::Point3f
  {
    return a + t * b;
  }
};
}  // namespace partou
