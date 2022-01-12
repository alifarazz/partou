#pragma once

#include "./utils.hh"
#include "material.hh"
//
#include "../random/random.hh"
#include "../shapes/hitable.hh"  // for hit_info definition

namespace partou
{
class Dielectric : public Material
{
public:
  Dielectric(math::Float ior)
      : ior {ior}
  {
  }

  auto scatter(const Ray& r_in, const hit_info& info, Spectrum& attenuation, Ray& r_scattered) const
      -> bool final override
  {
    using namespace partou::math;
    using namespace partou::utils;

    attenuation = Spectrum {1};

    const auto refraction_ratio = info.is_front_facing ? (Float(1) / ior) : ior;

    const auto iw = r_in.dir().normalized();
    const auto cos_theta = std::min(-iw.dot(info.normal), Float(1));
    const auto sin_theta = sqrt(1 - pow2(cos_theta));

    const auto fresnel_reflectance_probability = fresnel_reflectance(cos_theta, ior);

    const auto ow =
        (refraction_ratio * sin_theta < 1  // total internal reflection check
         || fresnel_reflectance_probability
                > random::unit<Float>()  // reflect if iw too close to macrosurface normal
         )
            ? refract(iw, info.normal, refraction_ratio, cos_theta)
            : reflect(iw, info.normal);  // internal reflection
    r_scattered = Ray(info.p, ow);
    return true;
  }

  math::Float ior;

protected:
  inline static auto fresnel_reflectance(math::Float cos_theta, math::Float ior) -> math::Float
  {
    // using Schlick's approximation
    // https://graphicscompendium.com/raytracing/11-fresnel-beer
    using namespace partou::math;

    const auto f0 = pow2((1 - ior) / (1 + ior));
    return f0 + (1 - f0) * std::pow(1 - cos_theta, 5);
  }
};
}  // namespace partou
