#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include "../math/vec.hh"

namespace partou
{
namespace io
{
class OBJ_Loader
{
public:
  std::vector<math::Point3f> m_vertices;
  std::vector<std::array<std::size_t, 3>> m_faces;

  OBJ_Loader() = default;
  explicit OBJ_Loader(const std::filesystem::path& path);

  int load(const std::filesystem::path& path);

protected:
  std::size_t getVertexIndex(const std::string& s);
};
}  // namespace io
}  // namespace partou
