#pragma once

#include <memory>

#include "../color/color.hh"
#include "../ray/ray.hh"

namespace partou
{

namespace sampling
{
class PDF;
}

struct scatter_info
{
  bool is_specular;
  std::shared_ptr<sampling::PDF> pdf_ptr;
  Spectrum attenuation;
  Ray specular_ray;
};
struct hit_info;

class Material
{
public:
  virtual auto scatter(const Ray&, const hit_info&, scatter_info&) const -> bool
  {
    return false;
  }

  virtual auto scattering_pdf(const Ray&, const hit_info&, const Ray&) const -> math::Float
  {
    return 0;
  }

  virtual auto emitted(const math::Vec2f&, const hit_info&, const math::Point3f&) const -> Spectrum
  {  // uv, hinfo, p
    return Spectrum {0};
  }
};
}  // namespace partou
