#pragma once

#include "../math/general.hh"
#include "color.hh"

namespace partou::tonemapper
{
class Gamma2
{
public:
  Gamma2(sRGBSpectrum spectrum, int sample_per_pixel)
      : m_spectrum {spectrum}
      , m_scale {math::Float(1.0) / sample_per_pixel}
  {
  }

  auto sRGB() const -> sRGBSpectrum
  {
    auto r = std::sqrt(m_scale * m_spectrum[0]), g = std::sqrt(m_scale * m_spectrum[1]),
         b = std::sqrt(m_scale * m_spectrum[2]);
    const auto fix_nan = [](const auto f) { return f == f ? f : 0; };  // zero out nan values
    r = fix_nan(r);
    g = fix_nan(g);
    b = fix_nan(b);
    return {math::clamp(math::Vec3f {r, g, b})};
  }

  sRGBSpectrum m_spectrum;
  math::Float m_scale;
};
}  // namespace partou::tonemapper
