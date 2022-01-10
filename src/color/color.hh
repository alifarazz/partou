#pragma once

#include "../math/general.hh"
#include "../math/vec.hh"
namespace partou
{
using sRGB8Spectrum = math::Vec3i;
using sRGBSpectrum = math::Vec3f;
using Spectrum = sRGBSpectrum;

inline static auto sRGB_2_sRGB8bit(const sRGBSpectrum& srgb) -> sRGB8Spectrum
{
  using namespace math;
  constexpr Float eps = 1e-4;
  constexpr Float low = 0;
  constexpr Float high = 1.0 - eps;
  auto r = math::clamp(srgb[0], low, high), g = math::clamp(srgb[1], low, high),
       b = math::clamp(srgb[2], low, high);
  return {int(r * 256), int(g * 256), int(b * 256)};
}
}  // namespace partou
