#pragma once

#include <Imath/ImathVec.h>

namespace partou
{
using Float = float;
using Double = double;
using Short = std::int16_t;
using Int = std::int32_t;
using Long = std::int64_t;

using Vec2i = Imath::Vec2<Int>;
using Vec2s = Imath::Vec2<Short>;
using Vec2f = Imath::Vec2<Float>;
using Vec2d = Imath::Vec2<Double>;

using Vec3i = Imath::Vec3<Int>;
using Vec3s = Imath::Vec3<Short>;
using Vec3f = Imath::Vec3<Float>;
using Vec3d = Imath::Vec3<Double>;

using Vec4i = Imath::Vec4<Int>;
using Vec4s = Imath::Vec4<Short>;
using Vec4f = Imath::Vec4<Float>;
using Vec4d = Imath::Vec4<Double>;

}  // namespace partou
