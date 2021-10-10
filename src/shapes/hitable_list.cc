#include "hitable_list.hh"

namespace partou
{
bool HitableList::hit(const Ray& r,
                      Float t_min,
                      Float t_max,
                      hit_info& info) const
{
  hit_info temp_info;
  bool hit_anything = false;
  auto closest_so_far = t_max;

  for (int i = 0; i < count; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, temp_info)) {
      hit_anything = true;
      closest_so_far = temp_info.t;
      info = temp_info;
    }
  }

  return hit_anything;
}

// bool HitableList::bounding_box(Float t0, Float t1, AABB& box) const
// {
//   if (count < 1)
//     return false;

//   AABB tmp_box;

//   bool hitable1_true {list[0]->bounding_box(t0, t1, tmp_box)};
//   if (!hitable1_true)
//     return false;
//   else
//     box = tmp_box;

//   for (int i = 1; i < count; i++)
//     if (list[i]->bounding_box(t0, t1, tmp_box))
//       box = surrounding_box(box, tmp_box);
//     else
//       return false;
//   return true;
// }
}  // namespace partou
