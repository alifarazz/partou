#pragma once

#include <type_traits>

namespace partou
{
namespace math
{
template<typename T, typename FloatingType>
requires std::is_floating_point_v<FloatingType>
inline auto interpolate_linear(const T& a, const T& a_1, FloatingType t) -> T
{
  return a * t + (1 - t) * a_1;
}

template<typename T>
requires std::is_arithmetic_v<T>
inline auto pow2(T a)
{
  return a * a;
}

}  // namespace math
}  // namespace partou
