#pragma once

#include "../math/vec.hh"

namespace partou {
class Ray {
public:
  Ray() = default;
  Ray(const partou::Vec3f &a, const partou::Vec3f &b) : a{a}, b{b} {}

  [[nodiscard]] auto orig() const -> const partou::Vec3f& { return a; }
  [[nodiscard]] auto dir() const -> const partou::Vec3f& { return b; }
  [[nodiscard]] auto eval_at(Float t) const -> partou::Vec3f { return a + t * b; }

  partou::Vec3f a, b;
};
} // namespace partou
