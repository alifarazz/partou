#pragma once

#include <cmath>
#include <vector>

#include "../material/material.hh"
#include "../math/vec.hh"
#include "hitable.hh"
#include "triangle.hh"
//
#include "../io/obj.hh"

namespace partou
{
namespace shape
{
class Mesh : public Hitable
{
public:
  Mesh() = delete;
  explicit Mesh(const std::vector<Triangle>& tris, std::shared_ptr<Material> matp = nullptr);
  explicit Mesh(const io::loader::OBJ& objLoader, std::shared_ptr<Material> matp = nullptr);

  auto apply(const math::spatial::Transform& tModel) -> Mesh&&;
  auto computeBoundingBox() -> void;

  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool final override;
  auto transformModel(const math::spatial::Transform& tModel) -> void final override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  std::shared_ptr<Material> m_matptr;
  std::vector<Triangle> m_tris;

protected:
};
}  // namespace shape
}  // namespace partou
