#pragma once

#include "../math/general.hh"
#include "color.hh"
//
#include "../perf_stats/stats.hh"

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
    auto has_nan = false;
    const auto fix_nan = [&has_nan](const auto f)
    {
      if (f == f)
        return f;
      has_nan = true;
      return decltype(f)(0);
    };  // zero out nan values
    r = fix_nan(r);
    g = fix_nan(g);
    b = fix_nan(b);

    stats::numNaNpixels += has_nan;

    return {math::clamp(math::Vec3f {r, g, b})};
  }

  sRGBSpectrum m_spectrum;
  math::Float m_scale;
};
}  // namespace partou::tonemapper
