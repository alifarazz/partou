#pragma once

#include "hitable.hh"

namespace partou
{
class HitableList : public Hitable
{
public:
  HitableList() {}
  HitableList(Hitable** l, int n)
      : list {l}
      , count {n}
  {
  }
  bool hit(const Ray& r,
           Float t_min,
           Float t_max,
           hit_info& info) const override;
  // virtual bool bounding_box(Float t0, Float t1, AABB& box) const;

  Hitable** list;
  int count;
};

}  // namespace partou
