#pragma once

#include "../math/general.hh"
#include "../math/onb.hh"
#include "../math/vec.hh"
#include "../random/random.hh"

namespace partou::sampling
{
class PDF
{
public:
  ~PDF() = default;

  virtual auto value(const math::Vec3f dir) const -> math::Float = 0;
  virtual auto generate() const -> math::Vec3f = 0;
};

class CosinePDF : public PDF
{
public:
  CosinePDF(const math::Vec3f& n)
      : onb {n}
  {
  }

  auto value(const math::Vec3f dir) const -> math::Float final override
  {
    const auto cosine = dir.normalized().dot(onb.n);
    return std::max(cosine / math::PI, math::Float(0));
  }
  auto generate() const -> math::Vec3f final override
  {
    return onb.local(random::cosine_direction<math::Float>());
  }

  math::spatial::ONB onb;
};
}  // namespace partou::sampling
