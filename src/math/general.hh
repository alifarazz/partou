#pragma once

#include <cmath>
#include <type_traits>

namespace partou::math
{
#ifdef FLOAT_IS_64_BIT
using Float = double;
#else
using Float = float;
#endif
using Double = double;

using Degree = Float;
using Radian = Float;

inline constexpr Float PI = 3.14159265358979323846;  // std::numbers::pi
inline constexpr Float INV_PI = 0.31830988618379067154;
inline constexpr Float HALF_PI = 1.57079632679489661923;
inline constexpr Float TWO_PI = 6.283185307179586476925;
// inline constexpr double EPSILON = 1e-9; // every func can use its own eps, as it sees fit

template<typename InputType>
requires std::is_floating_point_v<InputType>
inline auto toRadians(InputType degrees)
{
  return (degrees * PI / 180);
}

template<typename T, typename FloatingType>
requires std::is_floating_point_v<FloatingType>
inline auto interpolate_linear(const T& a, const T& a_1, FloatingType t) -> T
{
  return a * t + (1 - t) * a_1;
}

template<typename T>
requires std::is_arithmetic_v<T>
inline auto pow2(const T a)
{
  return a * a;
}

template<typename T>
requires std::is_floating_point_v<T>
inline auto sqrt(const T a) -> T
{
  return std::sqrt(a);
}

template<typename T>
requires std::is_floating_point_v<T>
inline auto inv(const T a) -> T
{
  return T(1) / a;
}

template<typename T>
inline auto clamp(const T x, const T min, const T max) -> T
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

template<typename R, typename T, typename U>
requires std::is_floating_point_v<R>
inline auto percent(const T i, const U n) -> R
{
  return static_cast<R>(Float(i) / Float(n) * Float(100));
}

template<typename R, typename T, typename U>
requires std::is_integral_v<R>
inline auto percent(const T i, const U n) -> R
{
  return static_cast<R>(std::round((Float(i) / Float(n) * Float(100))));
}
}  // namespace partou::math
