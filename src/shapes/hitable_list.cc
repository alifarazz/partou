#include "hitable_list.hh"

#include "../accel/AABB.hh"

namespace partou
{

HitableList::HitableList(std::vector<std::shared_ptr<const Hitable>> hitable_vector)
    : m_hitables {std::move(hitable_vector)}
{
  computeBB();
}

auto HitableList::computeBB() -> void
{
  this->m_aabb = accel::AABB();
  for (const auto &h: m_hitables){
    this->m_aabb.merge(h->aabb());
  }
}

auto HitableList::hit(const Ray& r,
                      const math::Float t_min,
                      const math::Float t_max,
                      hit_info& info) const -> bool
{
  hit_info temp_hitinfo;
  bool ray_did_hit_something = false;
  auto closest_so_far = t_max;

  math::Float tBB;
  for (const auto& hitable : this->m_hitables) {
    if (hitable->aabb().intersect(r, tBB) && hitable->hit(r, t_min, closest_so_far, temp_hitinfo)) {
      ray_did_hit_something = true;
      closest_so_far = temp_hitinfo.t;
      info = temp_hitinfo;
    }
  }

  return ray_did_hit_something;
}

auto HitableList::begin()
{
  return m_hitables.begin();
}
auto HitableList::end()
{
  return m_hitables.end();
}

// bool HitableList::bounding_box(math::Float t0, math::Float t1, AABB& box) const
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
