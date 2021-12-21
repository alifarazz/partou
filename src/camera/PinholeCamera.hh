#pragma once

#include "../ray/ray.hh"
#include "Camera.hh"

namespace partou
{
class PinholeCamera : public Camera
{
protected:
  math::Point3f m_origin;
  math::Point3f m_lower_left_corner;
  math::Vec3f m_horizontal;  // u * width
  math::Vec3f m_vertical;  // v * height

public:
  PinholeCamera() = default;
  explicit PinholeCamera(
      math::Point3f lookFrom, math::Point3f lookAt, math::Vec3f vUp, math::Float verticalFOV, math::Float aspectRatio);

  auto make_ray(math::Float u, math::Float v) const -> Ray;
};
}  // namespace partou
