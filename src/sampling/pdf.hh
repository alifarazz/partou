#pragma once

#include <memory>

#include "../math/general.hh"
#include "../math/onb.hh"
#include "../math/vec.hh"
#include "../random/random.hh"
#include "../shapes/hitable.hh"

namespace partou::sampling
{
class PDF
{
public:
  ~PDF() = default;

  virtual auto value(const math::Vec3f& dir) const -> math::Float = 0;
  virtual auto generate() const -> math::Vec3f = 0;
};

class CosinePDF : public PDF
{
public:
  explicit CosinePDF(const math::Vec3f& n)
      : onb {n}
  {
  }

  auto value(const math::Vec3f& direction) const -> math::Float final override
  {
    const auto cosine = direction.normalized().dot(onb.n);
    return std::max(cosine / math::PI, math::Float(0));
  }
  auto generate() const -> math::Vec3f final override
  {
    return onb.local(random::cosine_direction<math::Float>());
  }

  math::spatial::ONB onb;
};

class HitablePDF : public PDF
{
public:
  explicit HitablePDF(std::shared_ptr<const Hitable> hitable_ptr, const math::Vec3f& origin)
      : origin {origin}
      , hitable_ptr {hitable_ptr}
  {
  }

  auto value(const math::Vec3f& direction) const -> math::Float final override
  {
    return hitable_ptr->pdf_value(origin, direction);
  }
  auto generate() const -> math::Vec3f final override
  {
    return hitable_ptr->random(origin);
  }

  math::Point3f origin;
  std::shared_ptr<const Hitable> hitable_ptr;
};

class MixturePDF : PDF
{
public:
  MixturePDF(std::shared_ptr<const PDF> pdf0,
             std::shared_ptr<const PDF> pdf1)  // TODO:use initializer list
      : p0 {pdf0}
      , p1 {pdf1}
  {
  }

  auto value(const math::Vec3f& direction) const -> math::Float final override
  {  // half of each should do the trick
    return 0.5 * this->p0->value(direction) + 0.5 * this->p1->value(direction);
  }

  auto generate() const -> math::Vec3f final override
  {
    if (random::get(0, 1))  // coin toss
      return p1->generate();
    return p0->generate();
  }

  std::shared_ptr<const PDF> p0, p1;
};

}  // namespace partou::sampling
