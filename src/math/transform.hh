#pragma once
#include <type_traits>

#include "mat.hh"
#include "vec.hh"

namespace partou::math::spatial
{
class Transform
{
public:
  explicit Transform(const Vec3f& positionOffsetXYZ = Vec3f {0},
                     const Vec3f& scaleXYZ = Vec3f {1},
                     const Vec3f& rotatationAroundOriginXYZ = Vec3f {0});

  template<typename T>
  requires std::is_floating_point_v<T>
  auto transformPoint(const Point3<T>& p) const -> Point3<T>
  {
    Point3<T> ret;
    m_transform_matrix_.multVecMatrix(p, ret);
    return ret;
  }

  // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/transforming-normals
  // https://github.com/graphitemaster/normals_revisited
  template<typename T>
  requires std::is_floating_point_v<T>
  auto transformNormal(const Vec3<T>& n) const -> Vec3<T>
  {
    Vec3<T> ret;
    m_RS_matrix_.multVecMatrix(n, ret);
    ret.normalize();  // NOTE: Do I really need to normalize it? I think partou's presumption is
                      // that none of the normals are normalized.
    return (m_rotationMatrixHasNegativeDeterminant) ? ret * -1 : ret;
  }

  const Vec3f m_translation, m_scale, m_rotation;
  bool m_rotationMatrixHasNegativeDeterminant;

protected:
  Mat44f m_transform_matrix_, m_RS_matrix_;
};
}  // namespace partou::math::spatial
