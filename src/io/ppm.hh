#pragma once

#include <concepts>
#include <ostream>

#include "../math/vec.hh"
//
#include "../color/color.hh"
#include "../color/gamma2.hh"
#include "../film/film_buffer.hh"
//
#include "../utils/progress_bar.hh"  // for concepts

namespace partou::io
{
template<typename PixelType>  // sprinkle some concepts
requires std::is_convertible_v<PixelType, math::Vec3d>
class PPMImageSaver
{
public:
  // constructor
  PPMImageSaver() = default;
  explicit PPMImageSaver(FilmBuffer<PixelType>& filmbuffer)
      : filmbuffer {filmbuffer}
  {
  }

  auto save(std::ostream& out) const -> void
  {
    cli::ProgressBar pb {filmbuffer.buf.size() - 1, "PPMImageSaver::save->\tProgress", 4};
    out << "P3\n" << filmbuffer.nx() << " " << filmbuffer.ny() << "\n255\n";

    std::size_t i {};
    int spp = math::pow2(filmbuffer.sample_per_pixel_sqrt);
    for (const auto& pixel_color : filmbuffer.buf) {  // should've used rsv::enumerate
      const auto c_gm2 = tonemapper::Gamma2(pixel_color, spp).sRGB();
#ifdef CHECK_SRGB_VALUES
      if (c_gm2.x > 1 || c_gm2.y > 1 || c_gm2.z > 1 || c_gm2.x < 0 || c_gm2.y < 0 || c_gm2.z < 0) {
        const auto mesg = make_string_error_message_out_of_bounds(i, c_gm2);
        throw std::invalid_argument {mesg};
      }
#endif
      sRGB8Spectrum srgb = sRGB_2_sRGB8bit(c_gm2);
      out << srgb[0] << ' ' << srgb[1] << ' ' << srgb[2] << '\n';
      pb.tick();
      i++;
    }
    std::cerr << std::endl;
  }

  FilmBuffer<PixelType>& filmbuffer;

protected:
  static constexpr auto rgb_256_max_minus_eps = 255.999;

  auto make_string_error_message_out_of_bounds(std::size_t i, const Spectrum& pixel_color) const
  {
    using namespace std::string_literals;
    const auto yx = filmbuffer.get_yx(i);
    const auto coord_s = "("s + std::to_string(yx.x) + ", "s + std::to_string(yx.y) + ")"s;
    const auto rgb_s = std::to_string(pixel_color.x) + " "s + std::to_string(pixel_color.y) + " "s
                     + std::to_string(pixel_color.z);
    return "bad rgb value: "s + coord_s + ": "s + rgb_s;
  }
};
}  // namespace partou::io
