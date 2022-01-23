#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "obj.hh"

namespace partou::io::loader
{
OBJ::OBJ(const std::filesystem::path& path, bool load_normals)
{
  OBJ {};
  this->load(path, load_normals);
}
int OBJ::load(const std::filesystem::path& path, bool load_normals)
{
  if (!std::filesystem::exists(path)) {
    std::cerr << std::endl << "OBJ::load, " << path.string() << " not found.\n";
    return -2;
  }
//   std::cout << "OBJ::load-> begin\t" << path.filename() << '\n';

  std::ifstream file {path};
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss {line};
    std::string lineType;
    ss >> lineType;
    if (lineType == "o") {
      ss >> m_name;
    } else if (lineType == "v") {
      this->addVertex(ss);  // assert true
    } else if (lineType == "vn") {
      if (load_normals)
        this->addVertexNormal(ss);
    } else if (lineType == "f") {
      this->addTriangularFace(ss, load_normals);  // assert true
    }
  }

  std::cout << "OBJ::load-> done  \t" << path.filename() << '\n';
  return 0;
}

auto OBJ::addTriangularFace(std::stringstream& ss, bool load_normals) -> bool
{
  const auto isInvalid = [](auto arr) { return arr[0] == 0 || arr[1] == 0 || arr[2] == 0; };

  std::string s1, s2, s3;
  ss >> s1 >> s2 >> s3;

  std::array<std::size_t, 3> iV, iVN;  // iVT
  iV = {getIndexTriplet(s1), getIndexTriplet(s2), getIndexTriplet(s3)};
  // iVT = {getVertexIndex(s1, 1), getVertexIndex(s2, 1), getVertexIndex(s3, 1)};
  if (load_normals)
    iVN = {getIndexTriplet(s1, 2), getIndexTriplet(s2, 2), getIndexTriplet(s3, 2)};

  /*
  // HACK HACK HACK
  // std::cout << iVN[0] << " " << iVN[1] << " " << iVN[2] << std::endl;
  // static int i = 0;
  // if (i++ == (2977 - 2011  + 1))
  //   exit(-1);
  // HACK HACK HACK
  */
  if (isInvalid(iV) || (load_normals && isInvalid(iVN))) {
    std::cerr << "OBJ_Loader::addTriangularFace-> "
              << "Error: invalid vertex indices or normal indices" << std::endl;
    return false;
  }

  m_facesV.push_back(iV);
  if (load_normals)
    m_facesVN.push_back(iVN);
  return true;

  // for (const auto i : indices) // std::size_t is unsigned
  //   if (i < 0) {
  //     std::cerr << "OBJ::load::face, bad idx.\n";
  //     return -1;
  //   }
}

auto OBJ::addVertex(std::stringstream& ss) -> bool
{
  math::Point3f a;
  ss >> a.x >> a.y >> a.z;
  m_vertices.push_back(a);
  return true;
}

auto OBJ::addVertexNormal(std::stringstream& ss) -> bool
{
  math::Point3f a;
  ss >> a.x >> a.y >> a.z;
  m_vertexNormals.push_back(a);
  return true;
}

std::size_t OBJ::getIndexTriplet(const std::string& s, unsigned int location) const
{
  std::stringstream ss(s);
  std::string token;
  do {
    if (!std::getline(ss, token, '/'))
      return -1;
  } while (location--);
  return std::atol(token.data());
}

auto OBJ::loaded_normals() const -> bool
{
  return m_vertexNormals.size() != 0;
}
}  // namespace partou::io::loader
