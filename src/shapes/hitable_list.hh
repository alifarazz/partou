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
      -> bool override;
  // virtual bool bounding_box(math::Float t0, math::Float t1, AABB& box) const;

  auto begin();
  auto end();

  std::vector<std::shared_ptr<const Hitable>> m_hitables;

    protected:
    auto computeBB() -> void;
};

}  // namespace partou
