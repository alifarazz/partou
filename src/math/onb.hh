#pragma once

#include "vec.hh"

namespace partou::math::spatial
{
class ONB
{  // Ortho Normal Basis
public:
  ONB() = default;
  ONB(const Vec3f& normal);

  Vec3f local(Float a, Float b, Float c) const;
  Vec3f local(const Vec3f& abc) const;

  Vec3f s, t, n;
};
}  // namespace partou::math::spatial
