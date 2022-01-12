#pragma once
#include "../math/vec.hh"

namespace partou::utils
{

inline auto reflect(const math::Vec3f& iw, const math::Vec3f& n) -> math::Vec3f
{
  return iw - n * iw.dot(n) * 2;
}

inline auto refract(const math::Vec3f& iw,
                    const math::Vec3f& n,
                    const math::Float etai_over_etao,
                    const math::Float cos_theta)
{
  using namespace partou::math;

  /////////////////////////////////////////////////////////////////
  // ow = ?                                                      //
  // * sin o_theta = i_eta / o_eta * sin i_theta                 //
  // * etai_over_etao = i_eta / o_eta                            //
  // * ow = ow_prep + ow_parallel                                //
  // * ow_perp = etai_over_etao * ( iw + n * cos i_theta )       //
  // * ow_parallel = n * -sqrt(|1 - ow_perp.len2|) // Pythagoras //
  /////////////////////////////////////////////////////////////////
  const auto ow_perp = etai_over_etao * (iw + cos_theta * n);
  const auto ow_parallel = n * -std::sqrt(std::abs(1 - ow_perp.length2()));
  return ow_perp + ow_parallel;
}

inline auto refract(const math::Vec3f& iw, const math::Vec3f& n, const math::Float etai_over_etao)
    -> math::Vec3f
{
  const auto cos_theta = std::min(-iw.dot(n), math::Float(1));
  return refract(iw, n, etai_over_etao, cos_theta);
}
}  // namespace partou::utils
