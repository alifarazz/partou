#include <limits>

#include "mesh.hh"

#include "../random/random.hh"

namespace partou::shape
{
Mesh::Mesh(const std::vector<Triangle>& tris, std::shared_ptr<Material> matp)
    : m_matptr {matp}
    , m_tris {tris}
{
  this->computeBoundingBox();
};

Mesh::Mesh(const io::loader::OBJ& objLoader, std::shared_ptr<Material> matp)
    : m_matptr {matp}
{
  m_tris.resize(objLoader.m_facesV.size());
  if (!objLoader.loaded_normals()) {  // don't have normals -> flat shading
    for (std::size_t i = 0; i < objLoader.m_facesV.size(); i++) {
      const auto iV = objLoader.m_facesV[i];
      m_tris[i] = Triangle(objLoader.m_vertices[iV[0] - 1],
                           objLoader.m_vertices[iV[1] - 1],
                           objLoader.m_vertices[iV[2] - 1],
                           nullptr);
    }
  } else {  // do have normals -> smooth shading
    if (objLoader.m_facesV.size() != objLoader.m_facesVN.size()) {
      std::cerr << "Mesh::Mesh(const io::loader::OBJ& objLoader) -> Error\n\t-> "
                << "m_facesV.size != m_facesVN.size (" << objLoader.m_facesV.size()
                << " != " << objLoader.m_facesVN.size() << ")\n\t"
                << "OBJ name: " << objLoader.m_name << std::endl;
      exit(-1);
      return;
    }
    for (std::size_t i = 0; i < objLoader.m_facesV.size(); i++) {
      const auto iV = objLoader.m_facesV[i];
      const auto iVN = objLoader.m_facesVN[i];
      m_tris[i] = Triangle(objLoader.m_vertices[iV[0] - 1],
                           objLoader.m_vertices[iV[1] - 1],
                           objLoader.m_vertices[iV[2] - 1],
                           objLoader.m_vertexNormals[iVN[0] - 1],
                           objLoader.m_vertexNormals[iVN[1] - 1],
                           objLoader.m_vertexNormals[iVN[2] - 1],
                           nullptr);
    }
  }

  this->computeBoundingBox();
}

auto Mesh::apply(const math::spatial::Transform& tModel) -> Mesh&
{
  this->transformModel(tModel);
  return *this;
}

auto Mesh::transformModel(const math::spatial::Transform& tModel) -> void
{
  for (auto& tri : m_tris)
    tri.transformModel(tModel);
  computeBoundingBox();
}

void Mesh::computeBoundingBox()
{
  for (const auto& tri : m_tris)
    this->m_aabb.merge(tri.aabb());
}

auto Mesh::hit(const Ray& r, const math::Float t_min, const math::Float t_max, hit_info& info) const
    -> bool
{
  hit_info temp_hitinfo;
  bool ray_did_hit_something = false;
  auto closest_so_far = t_max;

  int i {};
  math::Float tBB;
  for (const Triangle& tri : this->m_tris) {
    if (tri.aabb().intersect(r, tBB) && tri.hit(r, t_min, closest_so_far, temp_hitinfo)) {
      ray_did_hit_something = true;
      closest_so_far = temp_hitinfo.t;
      info = temp_hitinfo;
      info.mat_ptr = m_matptr;
    }
    i++;
  }

  return ray_did_hit_something;
}

auto Mesh::pdf_value(const math::Point3f& origin, const math::Vec3f& dir) const -> math::Float
{
  const auto weight = math::Float(1) / m_tris.size();
  math::Float sum = 0;
  for (const auto& tri : m_tris)
    sum += weight * tri.pdf_value(origin, dir);
  return sum;
}

auto Mesh::random(const math::Point3f& origin) const -> math::Vec3f
{  // HACK: idk, return whatever
  const auto sz = static_cast<int>(m_tris.size());
  return m_tris[random::get(0, sz - 1)].random(origin);
}
}  // namespace partou::shape
