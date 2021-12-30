#pragma once

// #include <concepts>
// #include <type_traits>

#include "../math/vec.hh"
#include "../ray/ray.hh"
#include "../shapes/hitable.hh"

namespace partou
{
class Camera
{
};

namespace Concepts
{
// template<class CameraType>
// concept ICCamera =
//     std::is_base_of_v<Camera, CameraType> and requires(CameraType cam)
// {
//   std::is_same<Ray,
//                decltype(cam.make_ray(std::declval<Float>(),
//                                      std::declval<Float>()))>();
// };
}  // namespace Concepts
}  // namespace partou
