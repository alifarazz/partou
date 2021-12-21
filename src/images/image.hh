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
  std::size_t m_stride{};

public:
  std::vector<PixelType> buf;

  // constructor
  FilmBuffer() = default;

  explicit FilmBuffer(Vec2i ny_nx)
      : FilmBuffer(ny_nx.y, ny_nx.x) {};

  template<typename T>
  requires std::is_integral_v<T> FilmBuffer(T ny, T nx)
      : m_stride {static_cast<std::size_t>(nx)}
  {
    auto n_rows = static_cast<std::size_t>(ny);
    buf.resize(n_rows * m_stride);
  }

  // 2D array to flat array and back using row-major element access
  [[nodiscard]] auto get_idx(const Vec2i& yx) const -> std::size_t
  {
    return yx.x * m_stride + yx.y;
  }
  [[nodiscard]] auto get_yx(std::size_t idx) const -> Vec2i
  {
    auto y = static_cast<int>(idx / m_stride);
    auto x = static_cast<int>(idx % m_stride);
    return {y, x};
  }
  //
  auto find_pixel(Vec2i yx) -> PixelType&
  {
    auto idx = get_idx(yx);
    return buf[idx];
  }

  // getter
  auto pixel_color(Vec2i coord_yx) -> PixelType&
  {
    return this->find_pixel(coord_yx);
  }
  auto pixel_color(int ny, int nx) -> PixelType&
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
    return m_stride;
  }
  auto ny() const
  {
    return buf.size() / m_stride;
  }
};
}  // namespace partou
