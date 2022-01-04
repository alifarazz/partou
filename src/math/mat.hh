#pragma once

#include <Imath/ImathMatrix.h>

#include "general.hh"
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/vec4.hpp>

namespace partou::math
{
using Mat44d = Imath::Matrix44<Double>;
using Mat33d = Imath::Matrix33<Double>;
using Mat22d = Imath::Matrix22<Double>;
using Mat44f = Imath::Matrix44<Float>;
using Mat33f = Imath::Matrix33<Float>;
using Mat22f = Imath::Matrix22<Float>;
}  // namespace partou::math
