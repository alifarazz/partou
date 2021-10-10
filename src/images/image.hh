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

  PixelType& find_pixel(Vec2i yx)
  {
    auto idx = yx.x * stride + yx.y;
    return buf[idx];
  }

public:
  std::vector<PixelType> buf;

  // constructor
  FilmBuffer(int ny, int nx)
      : stride {nx}
  {
    buf.resize(ny * nx);
  };
  FilmBuffer(Vec2i ny_nx)
      : FilmBuffer(ny_nx.y, ny_nx.x) {};
  FilmBuffer() {};

  // getter
  PixelType& pixel_color(Vec2i coord_yx)
  {
    return this->find_pixel(coord_yx);
  }
  PixelType& pixel_color(int ny, int nx)
  {
    return this->pixel_color(Vec2i {ny, nx});
  }

  // setter
  void pixel_color(int ny, int nx, const PixelType& pixel_data) const
  {
    this->pixel_color(Vec2i {ny, nx}, pixel_data);
  }
  void pixel_color(Vec2i coord_yx, const PixelType& pixel_data) const
  {
    this->find_pixel(coord_yx) = pixel_data;
  }

  // common attrs
  auto nx() const
  {
    return stride;
  }
  auto ny() const
  {
    return buf.size() / stride;
  }
};
}  // namespace partou
