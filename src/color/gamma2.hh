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
    return {
        std::sqrt(m_scale * m_spectrum[0]),
        std::sqrt(m_scale * m_spectrum[1]),
        std::sqrt(m_scale * m_spectrum[2]),
    };
  }

  sRGBSpectrum m_spectrum;
  math::Float m_scale;
};
}  // namespace partou::tonemapper
