#pragma once

#include "general.hh"
#include <Imath/ImathVec.h>
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

using Point3f = Vec3f;
using Point3d = Vec3d;

}  // namespace partou
