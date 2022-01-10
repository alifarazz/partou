#pragma once

#include "../ray/ray.hh"
#include "../color/color.hh"

namespace partou
{
struct hit_info;

class Material
{
public:
  virtual auto scatter(const Ray& r_in,
                       const hit_info& info,
                       Spectrum& atttenuation,
                       Ray& r_scattered) const -> bool = 0;
};
}  // namespace partou
