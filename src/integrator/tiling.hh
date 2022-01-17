#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <vector>

#include "../camera/pinhole_camera.hh"
#include "../color/color.hh"
#include "../film/film_buffer.hh"
#include "../material/material.hh"
#include "../math/general.hh"
#include "../shapes/hitable.hh"
//
#include "uni_path_tracer.hh"

namespace partou::tiling
{
constexpr int_fast32_t TILESIZE = 16;

constexpr int make_tile_friendly(const int x)
{
  auto mod = x % TILESIZE;
  return mod ? x - mod + TILESIZE : x;
}

template<typename T>
static inline auto snap_tile(FilmBuffer<T>& fb,
                             const PinholeCamera& cam,
                             const Hitable& world,
                             std::shared_ptr<const Hitable>& lights,
                             const math::Vec2i& resolution,
                             const math::Vec2i& tile_size,
                             const math::Vec2i& tile_offset) -> void
{
  using namespace partou::math;

  // render tile
  for (int j = 0; j < tile_size.y; j++)
    for (int i = 0; i < tile_size.x; i++) {
      const auto pixel_coord = tile_offset + Vec2i {j, i};
      fb.pixel_color(pixel_coord) = integrator::uniPathTracer::samplePixelJittered(
          cam, world, lights, pixel_coord, resolution, fb.sample_per_pixel_sqrt);
    }
}

template<typename T>
static auto serial_tile_snap(FilmBuffer<T>& fb,
                             const PinholeCamera& cam,
                             const Hitable& world,
                             std::shared_ptr<const Hitable>& lights) -> void
{
  using namespace partou::math;

  auto report_progress = [](std::ostream& os, const auto i, const auto n)
  {
    os << "\rpartou::tiling::serial_tile_snap-> "
       << "tiles remaining: " << percent<int>(i, n) << "% (" << n - i << " out of " << n << ")"
       << std::flush;
  };

  const auto resolution = math::Vec2i {int(fb.nx()), int(fb.ny())};
  const Vec2i tile_size = resolution / TILESIZE;
  const auto tile_count = tile_size.x * tile_size.y;

  // render tiles
  for (int tile = 0; tile < tile_count; tile++) {
    report_progress(std::cout, tile, tile_count);
    const auto offset = Vec2i {tile % tile_size.x, tile / tile_size.y} * TILESIZE;
    snap_tile(fb, cam, world, lights, resolution, {TILESIZE, TILESIZE}, offset);
  }
}
}  // namespace partou::tiling
