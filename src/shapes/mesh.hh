#pragma once

#include <cmath>
#include <vector>

#include "hitable.hh"
#include "triangle.hh"
// #include "material.hxx"
#include "../math/vec.hh"
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
  explicit Mesh(const std::vector<Triangle>& tris);
  explicit Mesh(const io::OBJ_Loader& objLoader);

  void computeBB();

  auto hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
      -> bool override;
  // virtual bool bounding_box(float t0, float t1, AABB& box) const;

  std::vector<Triangle> m_tris;

protected:
};
}  // namespace shape
}  // namespace partou
