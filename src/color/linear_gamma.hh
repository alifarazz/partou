#pragma once

#include "../math/general.hh"
#include "color.hh"

namespace partou::tonemapper
{
class LinearGamma
{
public:
  LinearGamma(sRGBSpectrum spectrum, int sample_per_pixel)
      : m_spectrum {spectrum}
      , m_scale {math::Float(1.0) / sample_per_pixel}
  {
  }

  auto sRGB() const -> sRGBSpectrum
  {
    return {
        m_scale * m_spectrum[0],
        m_scale * m_spectrum[1],
        m_scale * m_spectrum[2],
    };
  }

  sRGBSpectrum m_spectrum;
  math::Float m_scale;
};
}  // namespace partou::tonemapper
