#pragma once

#include <concepts>
#include <ostream>

#include "../math/vec.hh"
//
#include "../film/film_buffer.hh"
#include "../utils/progress_bar.hh"
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
    for (const auto& pixel_color : filmbuffer.buf) {  // should've used rsv::enumerate
#ifdef CHECK_SRGB_VALUES
      constexpr auto rgbEpsilon = 0.000001;  // Increase if `out-of-bounds` throw happens
      if (pixel_color.x > 1. + rgbEpsilon || pixel_color.y > 1. + rgbEpsilon
          || pixel_color.z > 1. + rgbEpsilon || pixel_color.x < -rgbEpsilon
          || pixel_color.y < -rgbEpsilon || pixel_color.z < -rgbEpsilon)
      {
        const auto mesg = make_string_error_message_out_of_bounds(i, pixel_color);
        throw std::invalid_argument {mesg};
      }
#endif
      const math::Vec3f rgb = pixel_color;
      out << static_cast<int>(rgb_256_max_minus_eps * rgb[0]) << ' '
          << static_cast<int>(rgb_256_max_minus_eps * rgb[1]) << ' '
          << static_cast<int>(rgb_256_max_minus_eps * rgb[2]) << '\n';
      pb.tick();
      i++;
    }
    std::cerr << std::endl;
  }

  FilmBuffer<PixelType>& filmbuffer;

protected:
  static constexpr auto rgb_256_max_minus_eps = 255.999;

  auto make_string_error_message_out_of_bounds(std::size_t i, const math::Vec3f& pixel_color) const
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
