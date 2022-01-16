#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "hitable.hh"

namespace partou
{
class HitableList : public Hitable
{
public:
  HitableList() = delete;
  explicit HitableList(std::vector<std::shared_ptr<const Hitable>> hitable_vector);

  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool final override;
  auto transformModel(const math::spatial::Transform& tModel) -> void final override;
  auto pdf_value(const math::Point3f& origin, const math::Vec3f& dir) const
      -> math::Float final override;
  auto random(const math::Point3f& origin) const -> math::Vec3f final override;

  auto begin();
  auto end();

  std::vector<std::shared_ptr<const Hitable>> m_hitables;

protected:
  auto computeBoundingBox() -> void;
};

}  // namespace partou
