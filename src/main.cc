#include <iostream>
#include <string>
#include <ranges>

#include "./utils/images/image.hh"
#include "./utils/images/ppm.hh"


auto main() -> int
{
  using namespace partou;


  Vec2i buffersize{10, 10};
  FilmBuffer<Vec3f> filmbuffer{buffersize};

  for (auto i : std::ranges::iota_view{2, 8})
    filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  PPMImageSaver{filmbuffer, std::cout}.save();
}
