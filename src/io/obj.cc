#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "obj.hh"

namespace partou
{
namespace io
{
OBJ_Loader::OBJ_Loader(const std::filesystem::path& path)
{
  OBJ_Loader {};
  this->load(path);
}
int OBJ_Loader::load(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path)) {
    std::cerr << std::endl << "OBJ::load, " << path.string() << " not found.\n";
    return -2;
  }
  std::cerr << "OBJ::load-> begin\t" << path.filename() << '\n';

  std::ifstream file {path};
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss {line};
    std::string lineType;
    ss >> lineType;
    if (lineType == "v") {
      math::Point3f a;
      ss >> a.x >> a.y >> a.z;
      m_vertices.push_back(a);
    } else if (lineType == "f") {
      std::string s1, s2, s3;
      ss >> s1 >> s2 >> s3;
      std::array<std::size_t, 3> indices {
          getVertexIndex(s1) - 1, getVertexIndex(s2) - 1, getVertexIndex(s3) - 1};
      // for (const auto i : indices) // std::size_t is unsigned
      //   if (i < 0) {
      //     std::cerr << "OBJ::load::face, bad idx.\n";
      //     return -1;
      //   }
      m_faces.push_back(indices);
    }
  }

  std::cerr << "OBJ::load-> end  \t" << path.filename() << '\n';
  return 0;
}

std::size_t OBJ_Loader::getVertexIndex(const std::string& s)
{
  std::stringstream ss(s);
  std::string token;
  if (std::getline(ss, token, '/'))
    return std::atol(token.data());
  return -1;
}
}  // namespace io
}  // namespace partou
