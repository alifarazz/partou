#pragma once

#include <vector>

#include "../math/vec.hh"

namespace partou
{
template<typename PixelType>
class FilmBuffer
{
private:
  const int stride;

  PixelType& find_pixel(Vec2i hw)
  {
    auto idx = hw.x * stride + hw.y;
    return buf[idx];
  }

public:
  std::vector<PixelType> buf;

  // constructor
  FilmBuffer(int height, int width)
      : stride {width}
  {
    buf.resize(height * width);
  };
  FilmBuffer(Vec2i height_width)
      : FilmBuffer(height_width.x, height_width.y) {};
  FilmBuffer() {};

  // getter
  PixelType& pixel_color(Vec2i coord_hw)
  {
    return this->find_pixel(coord_hw);
  }
  PixelType& pixel_color(int height, int width)
  {
    return this->pixel_color(Vec2i {height, width});
  }

  // setter
  void pixel_color(int height, int width, const PixelType& pixel_data) const
  {
    this->pixel_color(Vec2i {height, width}, pixel_data);
  }
  void pixel_color(Vec2i coord_hw, const PixelType& pixel_data) const
  {
    this->find_pixel(coord_hw) = pixel_data;
  }

  // common attrs
  auto width() const
  {
    return stride;
  }
  auto height() const
  {
    return buf.size() / stride;
  }
};
}  // namespace partou
