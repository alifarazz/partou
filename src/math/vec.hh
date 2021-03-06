#pragma once

#include <Imath/ImathMatrix.h>
#include <Imath/ImathVec.h>

#include "general.hh"
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/vec4.hpp>

namespace partou::math
{

// there are useless
// using Short = std::int16_t;
// using Int = std::int32_t;
// using Long = std::int64_t;

// using Vec2l = glm::i64vec2;
// using Vec2i = glm::i32vec2;
// using Vec2s = glm::i16vec2;
// using Vec2f = glm::vec2;
// using Vec2d = glm::dvec2;

// using Vec3l = glm::i64vec3;
// using Vec3i = glm::i32vec3;
// using Vec3s = glm::i16vec3;
// using Vec3f = glm::vec3;
// using Vec3d = glm::dvec3;

// using Vec4l = glm::i64vec4;
// using Vec4i = glm::i32vec4;
// using Vec4s = glm::i16vec4;
// using Vec4f = glm::vec4;
// using Vec4d = glm::dvec4;

template<typename T>
using Vec2 = Imath::Vec2<T>;
template<typename T>
using Vec3 = Imath::Vec3<T>;
template<typename T>
using Vec4 = Imath::Vec4<T>;

using Vec2l = Imath::Vec2<std::int64_t>;
using Vec2i = Imath::Vec2<std::int32_t>;
using Vec2s = Imath::Vec2<std::int16_t>;
using Vec2f = Imath::Vec2<Float>;
using Vec2d = Imath::Vec2<Double>;

using Vec3l = Imath::Vec3<std::int64_t>;
using Vec3i = Imath::Vec3<std::int32_t>;
using Vec3s = Imath::Vec3<std::int16_t>;
using Vec3f = Imath::Vec3<Float>;
using Vec3d = Imath::Vec3<Double>;

using Vec4l = Imath::Vec4<std::int64_t>;
using Vec4i = Imath::Vec4<std::int32_t>;
using Vec4s = Imath::Vec4<std::int16_t>;
using Vec4f = Imath::Vec4<Float>;
using Vec4d = Imath::Vec4<Double>;

template<typename T>
using Point2 = Imath::Vec2<T>;
template<typename T>
using Point3 = Imath::Vec3<T>;
template<typename T>
using Point4 = Imath::Vec4<T>;

using Point3f = Point3<Float>;
using Point3d = Point3<Double>;

template<typename T>
requires std::is_floating_point_v<T>
inline static bool near_zero(const Vec3<T>& v)
{  // Return true if the vector is close to zero in all dimensions.
  constexpr auto eps = 1e-8;
  return v.equalWithAbsError(Vec3<T> {0, 0, 0}, eps);
  // bool is_zero = std::abs(v.x) < eps && std::abs(v.x) < eps && std::abs(v.x) < eps;
  // return is_zero;
}

template<typename T>
requires std::is_floating_point_v<T>
inline static auto clamp(const Vec3<T>& v) -> Vec3<T>
{
  const T low = 0, high = 1;
  return decltype(v) {
      std::clamp(v.x, low, high),
      std::clamp(v.y, low, high),
      std::clamp(v.z, low, high),
  };
}

template<typename T>
requires std::is_floating_point_v<T>
inline static auto sqrt(const Vec3<T>& v) -> Vec3<T>
{
  return {
      std::sqrt(v[0]),
      std::sqrt(v[1]),
      std::sqrt(v[2]),
  };
}

template<typename T>
requires std::is_floating_point_v<T>
inline static auto inv(const Vec2<T>& v) -> Vec2<T>
{
  return {inv(v[0]), inv(v[1])};
}
}  // namespace partou::math
