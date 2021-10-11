#pragma once

#include <type_traits>
#include <vector>

#include "../math/vec.hh"

namespace partou
{
template<typename PixelType>
class FilmBuffer
{
private:
  std::size_t stride;

public:
  std::vector<PixelType> buf;

  // constructor
  FilmBuffer() = default;

  FilmBuffer(Vec2i ny_nx)
      : FilmBuffer(ny_nx.y, ny_nx.x) {};

  template<typename T>
  requires std::is_integral_v<T> FilmBuffer(T ny, T nx)
      : stride {static_cast<std::size_t>(nx)}
  {
    auto n_rows = static_cast<std::size_t>(ny);
    buf.resize(n_rows * stride);
  }

  // 2D array to flat array and back using row-major element access
  std::size_t get_idx(Vec2i yx) const
  {
    return yx.x * stride + yx.y;
  }
  Vec2i get_yx(std::size_t idx) const
  {
    auto y = static_cast<int>(idx / stride);
    auto x = static_cast<int>(idx % stride);
    return {y, x};
  }
  //
  PixelType& find_pixel_(Vec2i yx)
  {
    auto idx = get_idx(yx);
    return buf[idx];
  }

  // getter
  PixelType& pixel_color(Vec2i coord_yx)
  {
    return this->find_pixel_(coord_yx);
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
    this->find_pixel_(coord_yx) = pixel_data;
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
