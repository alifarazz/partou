#pragma once

#include <cmath>
#include <vector>

#include "../material/material.hh"
#include "../math/vec.hh"
#include "hitable.hh"
#include "triangle.hh"
//
#include "../io/obj.hh"

namespace partou::shape
{
class Mesh : public Hitable
{
public:
  explicit Mesh() = default;
  explicit Mesh(const std::vector<Triangle>& tris, std::shared_ptr<Material> matp = nullptr);
  explicit Mesh(const io::loader::OBJ& objLoader, std::shared_ptr<Material> matp = nullptr);
  // explicit Mesh(Mesh& m, std::shared_ptr<Material> matp = nullptr);
  // explicit Mesh(Mesh&& m) = default;
  // explicit Mesh(const Mesh& m) = default;
  // explicit Mesh(Mesh& m);

  auto apply(const math::spatial::Transform& tModel) -> Mesh&;
  auto computeBoundingBox() -> void;

  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool final override;
  auto transformModel(const math::spatial::Transform& tModel) -> void final override;
  auto pdf_value(const math::Point3f& origin, const math::Vec3f& dir) const
      -> math::Float final override;
  auto random(const math::Point3f& origin) const -> math::Vec3f final override;

  std::shared_ptr<Material> m_matptr;
  std::vector<Triangle> m_tris;

protected:
};
}  // namespace partou::shape
