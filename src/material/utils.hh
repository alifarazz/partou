#pragma once
#include "../math/vec.hh"
namespace partou::utils
{

template<typename T>
requires std::is_floating_point_v<T>
auto reflect(const math::Vec3<T>& iw, const math::Vec3<T>& n)
{
  auto v = -iw;  // FIXME
  return v - n * v.dot(n) * 2;
}

}  // namespace partou::utils