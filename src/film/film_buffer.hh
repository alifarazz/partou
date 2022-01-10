#pragma once

#include <type_traits>
#include <vector>

#include "../math/vec.hh"

namespace partou
{
template<typename PixelType>
class FilmBuffer // TODO: rename class to Film
{
private:
  std::size_t m_stride {};

public:
  int sample_per_pixel_sqrt;
  std::vector<PixelType> buf;

  // constructor
  FilmBuffer() = default;

  explicit FilmBuffer(math::Vec2i ny_nx, int spp_sqrt = 1)
      : FilmBuffer(ny_nx.y, ny_nx.x, spp_sqrt) {};

  template<typename T, typename U>
  requires std::is_integral_v<T> && std::is_integral_v<U> FilmBuffer(T ny, T nx, U spp_sqrt = 1)
      : m_stride {static_cast<std::size_t>(nx)}
  {
    this->sample_per_pixel_sqrt = static_cast<int>(spp_sqrt);
    auto n_rows = static_cast<std::size_t>(ny);
    buf.resize(n_rows * m_stride);
  }

  // 2D array to flat array and back using row-major element access
  [[nodiscard]] auto get_idx(const math::Vec2i& yx) const -> std::size_t
  {
    return yx.x * m_stride + yx.y;
  }
  [[nodiscard]] auto get_yx(std::size_t idx) const -> math::Vec2i
  {
    auto y = static_cast<int>(idx / m_stride);
    auto x = static_cast<int>(idx % m_stride);
    return {y, x};
  }
  //
  auto find_pixel(math::Vec2i yx) -> PixelType&
  {
    auto idx = get_idx(yx);
    return buf[idx];
  }

  // getter
  auto pixel_color(math::Vec2i coord_yx) -> PixelType&
  {
    return this->find_pixel(coord_yx);
  }
  auto pixel_color(int ny, int nx) -> PixelType&
  {
    return this->pixel_color(math::Vec2i {ny, nx});
  }

  // setter
  void pixel_color(int ny, int nx, const PixelType& pixel_data) const
  {
    this->pixel_color(math::Vec2i {ny, nx}, pixel_data);
  }
  void pixel_color(math::Vec2i coord_yx, const PixelType& pixel_data) const
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
