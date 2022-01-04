#include "transform.hh"

namespace partou::math::spatial
{
Transform::Transform(const Vec3f& positionOffsetXYZ,
                     const Vec3f& scaleXYZ,
                     const Vec3f& rotatationAroundOriginXYZ)
    : m_translation {positionOffsetXYZ}
    , m_scale {scaleXYZ}
    , m_rotation {rotatationAroundOriginXYZ}
    , m_rotationMatrixHasNegativeDeterminant {m_scale.x * m_scale.y * m_scale.z < 0}
    , m_transform_matrix_ {Imath::UNINITIALIZED}  // HACK
    , m_RS_matrix_ {Imath::UNINITIALIZED}  // HACK
{
  m_transform_matrix_.makeIdentity();
  m_transform_matrix_.translate(m_translation);
  m_transform_matrix_.rotate(m_rotation);
  m_transform_matrix_.scale(m_scale);

  m_RS_matrix_.makeIdentity();
  m_RS_matrix_.rotate(m_rotation);
  m_RS_matrix_.scale(m_scale);
}
}  // namespace partou::math::spatial
