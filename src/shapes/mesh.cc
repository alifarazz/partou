#include <limits>

#include "mesh.hh"

namespace partou
{
namespace shape
{
Mesh::Mesh(const io::OBJ_Loader& objLoader)
{
  m_tris.resize(objLoader.m_faces.size());
  int i = 0;
  for (const auto& face : objLoader.m_faces) {
    m_tris[i++] = Triangle(objLoader.m_vertices[face[0]],
                           objLoader.m_vertices[face[1]],
                           objLoader.m_vertices[face[2]],
                           nullptr);
  }
}

auto Mesh::hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
    -> bool
{
  hit_info temp_hitinfo;
  bool ray_did_hit_something = false;
  auto closest_so_far = t_max;

  int i{};
  for (const auto& hitable : this->m_tris) {
    if (hitable.hit(r, t_min, closest_so_far, temp_hitinfo)) {
      ray_did_hit_something = true;
      closest_so_far = temp_hitinfo.t;
      info = temp_hitinfo;
      info.i = i;
    }
      i++;
  }

  return ray_did_hit_something;
}
}  // namespace shape
}  // namespace partou
