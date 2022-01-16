#include "hitable_list.hh"

#include "../accel/AABB.hh"
#include "../random/random.hh"

namespace partou
{
HitableList::HitableList(std::vector<std::shared_ptr<const Hitable>> hitable_vector)
    : m_hitables {std::move(hitable_vector)}
{
  computeBoundingBox();
}

auto HitableList::computeBoundingBox() -> void
{
  this->m_aabb = accel::AABB();
  for (const auto& h : m_hitables) {
    this->m_aabb.merge(h->aabb());
  }
}

auto HitableList::transformModel(const math::spatial::Transform& /* tModel */) -> void
{  // noop
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

auto HitableList::pdf_value(const math::Point3f& origin, const math::Vec3f& dir) const
    -> math::Float
{
  const auto weight = math::Float(1) / m_hitables.size();
  math::Float sum = 0;
  for (const auto& tri : m_hitables)
    sum += weight * tri->pdf_value(origin, dir);
  return sum;
}
auto HitableList::random(const math::Point3f& origin) const -> math::Vec3f
{  // HACK: again, idk, return whatever
  const auto sz = static_cast<int>(m_hitables.size());
  return m_hitables[random::get(0, sz - 1)]->random(origin);
}
}  // namespace partou
