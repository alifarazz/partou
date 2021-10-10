#pragma once

#include <concepts>
#include <ostream>

#include "../utils/progress_bar.hh"
#include "image.hh"

namespace partou
{
template<typename PixelType>  // sprinkle some concepts
class PPMImageSaver
{
public:
  FilmBuffer<PixelType>& filmbuffer;
  std::ostream& out;

  auto save()
  {
    cli::ProgressBar pb {filmbuffer.buf.size() - 1, "PPMImageSaver.save: "};
    out << "P3\n" << filmbuffer.nx() << " " << filmbuffer.ny() << "\n255\n";
    for (const auto& pixel_color : filmbuffer.buf) {
      // if (i % 32 == 0)
      out << static_cast<int>(255.999 * pixel_color.x) << ' '
          << static_cast<int>(255.999 * pixel_color.y) << ' '
          << static_cast<int>(255.999 * pixel_color.z) << '\n';
      pb.tick();
    }
    std::cerr << std::endl;
  }
};
}  // namespace partou
