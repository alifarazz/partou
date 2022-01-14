#pragma once

#include <random>
#include <type_traits>

#ifdef USE_PCG
#  include <pcg_random.hpp>
#endif

#include "../math/general.hh"
#include "../math/vec.hh"

namespace partou::random
{
// thread_local to create one differently seeded engine per thread
#ifdef USE_PCG
// may throw uncatchable exception
inline thread_local pcg32 engine {pcg_extras::seed_seq_from<std::random_device> {}};
#else
// may throw uncatchable exception
inline thread_local auto engine = std::mt19937(std::random_device {}());
#endif

template<typename T>
auto get(const T min, const T max) -> T
{
  if constexpr (std::is_integral<T>::value) {
    return std::uniform_int_distribution(min, max)(engine);  // [min, max]
  } else {
    return std::uniform_real_distribution(min, std::nextafter(max, min))(engine);  // [min, max)
  }
}

template<typename T>
requires std::is_floating_point_v<T>
auto unit() -> T
{
  static thread_local std::uniform_real_distribution<T> unit_distribution(
      0.0, std::nextafter(1.0, 0.0));  // [0.0, 1.0)
  return unit_distribution(engine);
}

template<typename T>
requires std::is_floating_point_v<T>
auto angle() -> T
{
  {
    static thread_local std::uniform_real_distribution<T> angle_distribution(
        0.0, std::nextafter(partou::math::TWO_PI, 0.0));
    return angle_distribution(engine);
  }
}

template<typename T>
requires std::is_floating_point_v<T>
auto vec_in_sphere() -> math::Vec3<T>
{  // using rejection sampling method
  math::Vec3<T> v;
  do {
    v[0] = get<T>(-1, 1);
    v[1] = get<T>(-1, 1);
    v[2] = get<T>(-1, 1);
  } while (v.length2() >= 1);
  return v;  // Cartesian coordinates
}

template<typename T>
requires std::is_floating_point_v<T>
auto vec_in_hemisphere(const math::Vec3<T>& normal) -> math::Vec3<T>
{
  const auto in_unit_sphere = vec_in_sphere<T>();
  return in_unit_sphere.dot(normal) > 0 ? in_unit_sphere : -in_unit_sphere;
}

template<typename T>
requires std::is_floating_point_v<T>
auto unit_vec3() -> math::Vec3<T>
{
  return vec_in_sphere<T>().normalize();  // Cartesian coordinates
}

/*
template <typename T>
requires std::is_floating_point_v<T>
auto point_in_circle() -> std::tuple<T>
  {
#define USE_REJECTION_SAMPLING_POINT_IN_CIRCLE  // is the fastest
#ifdef USE_REJECTION_SAMPLING_POINT_IN_CIRCLE

  // uses rejection sampling, expected value of the number of attempts = pi/4
  for(;;){
    auto u = unit<T>();
    auto v = unit<T>();
    if (u * u + v * v < 1)
      return {u,v};
  }

#elif USE_POLAR_INV_TRANS_SAMPLING_POINT_IN_CIRCLE
  // uses polar coordinates to choose sample, then uses it to do inverse transform sampling
  // more info: https://youtu.be/4y_nmpv-9lI
  auto r = unit<T>();
  r = std::sqrt(r); // inverse tranform sampling happens on this line
  auto theta = angle<T>();
  return {r * std::cos(theta), r * sin(theta)};
#elif USE_TRIANGLE_POINT_IN_CIRCLE
  auto theta = angle<T>();

  auto r = unit<T>() + unit<T>();
  r = r < 1 ?  r : 2 - r; // make sure it's inside the circle
  return {r * std::cos(theta), r * sin(theta)};
#elif USE_MAX_RADIUS_POINT_IN_CIRCLE
  auto theta = angle<T>();
  auto r = std::max(unit<T>(), unit<T>());
  return {r * std::cos(theta), r * sin(theta)};
#else
#error "No partou::random::point_in_circle() implementation selected"
#endif
  }
*/
}  // namespace partou::random
