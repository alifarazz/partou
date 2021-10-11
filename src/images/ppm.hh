#pragma once

#include <concepts>
#include <ostream>

#include <fmt/format.h>

#include "../utils/progress_bar.hh"
#include "image.hh"

namespace partou
{
template<typename PixelType>  // sprinkle some concepts
class PPMImageSaver
{
public:
  FilmBuffer<PixelType>& filmbuffer;

  auto save(std::ostream& out)
  {
    using namespace std::string_literals;
    cli::ProgressBar pb {filmbuffer.buf.size() - 1, "PPMImageSaver.save: "};
    out << "P3\n" << filmbuffer.nx() << " " << filmbuffer.ny() << "\n255\n";

    int i {};
    for (const auto& pixel_color : filmbuffer.buf) {
      if (pixel_color.x > 1. || pixel_color.y > 1. || pixel_color.z > 1.) {
        auto yx = filmbuffer.get_yx(i);
        auto coord_s =
            "("s + std::to_string(yx.x) + ", "s + std::to_string(yx.y) + ")"s;
        auto rgb_s = std::to_string(pixel_color.x) + " "s
            + std::to_string(pixel_color.y) + " "s
            + std::to_string(pixel_color.z);
        throw std::invalid_argument {"bad rgb value: " + coord_s + ": "s
                                     + rgb_s};
      }
      out << static_cast<int>(255.999 * pixel_color.x) << ' '
          << static_cast<int>(255.999 * pixel_color.y) << ' '
          << static_cast<int>(255.999 * pixel_color.z) << '\n';
      pb.tick();
      i++;
    }
    std::cerr << std::endl;
  }
};
}  // namespace partou
