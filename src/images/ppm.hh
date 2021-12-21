#pragma once

#include <concepts>
#include <ostream>

//
#include "../utils/progress_bar.hh"
#include "image.hh"
//
#include "../utils/progress_bar.hh"  // for concepts

namespace partou {
template <typename PixelType>  // sprinkle some concepts
requires std::is_convertible_v<PixelType, Vec3d>
class PPMImageSaver {
  static constexpr auto rgb_256_max_minus_eps = 255.999;

  auto make_throw_error_message_out_of_bounds(std::size_t i,
                                              const Vec3f& pixel_color) const {
    using namespace std::string_literals;
    const auto yx = filmbuffer.get_yx(i);
    const auto coord_s =
        "("s + std::to_string(yx.x) + ", "s + std::to_string(yx.y) + ")"s;
    const auto rgb_s = std::to_string(pixel_color.x) + " "s +
                       std::to_string(pixel_color.y) + " "s +
                       std::to_string(pixel_color.z);
    return "bad rgb value: "s + coord_s + ": "s + rgb_s;
  }

 public:
  FilmBuffer<PixelType>& filmbuffer;
  // constructor
  explicit PPMImageSaver(FilmBuffer<PixelType>& filmbuffer)
      : filmbuffer{filmbuffer} {}
  //
  auto save(std::ostream& out) const {
    cli::ProgressBar pb{filmbuffer.buf.size() - 1, "PPMImageSaver.save: ", 4};
    out << "P3\n" << filmbuffer.nx() << " " << filmbuffer.ny() << "\n255\n";

    std::size_t i{};
    for (const auto& pixel_color :
         filmbuffer.buf) {  // should've used rsv::enumerate
      if (pixel_color.x > 1. || pixel_color.y > 1. || pixel_color.z > 1.) {
        const auto mesg =
            make_throw_error_message_out_of_bounds(i, pixel_color);
        throw std::invalid_argument{mesg};
      }
      out << static_cast<int>(rgb_256_max_minus_eps * pixel_color.x) << ' '
          << static_cast<int>(rgb_256_max_minus_eps * pixel_color.y) << ' '
          << static_cast<int>(rgb_256_max_minus_eps * pixel_color.z) << '\n';
      pb.tick();
      i++;
    }
    std::cerr << std::endl;
  }
};
}  // namespace partou
