#include "pinhole_camera.hh"

namespace partou
{
PinholeCamera::PinholeCamera(math::Point3f lookFrom,
                             math::Point3f lookAt,
                             math::Vec3f vUp,
                             math::Float verticalFOV,
                             math::Float aspectRatio)
    : m_origin {lookFrom}
{
  const math::Float theta = math::toRadians(verticalFOV);
  const auto h = std::tan(theta / 2);
  const auto viewportH = 2 * h, viewportW = aspectRatio * viewportH;

  const auto viewDir = (lookAt - lookFrom).normalize();
  const auto u = viewDir.cross(vUp).normalize();
  const auto v = u.cross(viewDir);

  m_horizontal = viewportW * u;
  m_vertical = viewportH * v;
  m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 + viewDir;
  // The `+ viewDir` is for the hole of the pin-hole camera
}
auto PinholeCamera::make_ray(math::Float u, math::Float v) const -> Ray
{
  auto point_on_film = m_lower_left_corner + u * m_horizontal + v * m_vertical;
  auto point_on_hole = point_on_film - m_origin;
  return {m_origin, point_on_hole};
}
};  // namespace partou
