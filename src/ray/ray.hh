#pragma once

#include "../math/vec.hh"

namespace partou {
class Ray {
public:
  Ray() {}
  Ray(const partou::Vec3f &a, const partou::Vec3f &b) : A{a}, B{b} {}

  const partou::Vec3f& orig() const { return A; }
  const partou::Vec3f& dir() const { return B; }
  partou::Vec3f eval_at(Float t) const { return A + t * B; }

  partou::Vec3f A, B;
};
}
