#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include "../math/vec.hh"

namespace partou::io::loader
{
class OBJ
{
public:
  std::string m_name {"partou_empty_obj_name"};
  std::vector<math::Point3f> m_vertices {};
  std::vector<math::Point3f> m_vertexNormals {};
  std::vector<std::array<std::size_t, 3>> m_facesV {};
  std::vector<std::array<std::size_t, 3>> m_facesVN {};

  OBJ() = default;
  explicit OBJ(const std::filesystem::path& path, bool load_normals = false);

  int load(const std::filesystem::path& path, bool load_normals = false);
  bool loaded_normals() const;

protected:
  bool addVertex(std::stringstream& ss);
  bool addVertexNormal(std::stringstream& ss);
  bool addTriangularFace(std::stringstream& ss, bool load_normals);

  std::size_t getIndexTriplet(const std::string& token, unsigned int location = 0u) const;
};
}  // namespace partou::io::loader
