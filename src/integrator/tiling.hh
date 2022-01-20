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

// Thanks iq!
// all from https://www.iquilezles.org/www/articles/cputiles/cputiles.htm
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
                             const std::shared_ptr<const Hitable>& lights,
                             const math::Vec2i& resolution,
                             const math::Vec2i& tile_offset) -> void
{
  using namespace partou::math;

  // render tile
  for (int j = 0; j < TILESIZE; j++)
    for (int i = 0; i < TILESIZE; i++) {
      const auto pixel_coord = tile_offset + Vec2i {j, i};
      fb.pixel_color(pixel_coord) = integrator::uniPathTracer::samplePixelJittered(
          cam, world, lights, pixel_coord, resolution, fb.sample_per_pixel_sqrt);
    }
}

template<typename T>
static auto serial_tile_snap(FilmBuffer<T>& fb,
                             const PinholeCamera& cam,
                             const Hitable& world,
                             const std::shared_ptr<const Hitable>& lights) -> void
{
  using namespace partou::math;

  auto report_progress = [](std::ostream& os, const auto i, const auto n)
  {
    os << "\rtiling::serial_tile_snap-> "
       << "tiles remaining: " << percent<int>(i, n) << "% (" << n - i << " out of " << n << ")"
       << std::flush;
  };

  const auto resolution = math::Vec2i {int(fb.nx()), int(fb.ny())};
  const auto tile_size = resolution / TILESIZE;
  const auto tile_count = tile_size.x * tile_size.y;

  // render tiles
  for (int tile = 0; tile < tile_count; tile++) {
    report_progress(std::cout, tile, tile_count);
    auto&& offset = Vec2i {tile % tile_size.x, tile / tile_size.y} * TILESIZE;
    snap_tile(fb, cam, world, lights, resolution, std::move(offset));
  }
}

template<typename T>
static auto parallel_tile_snap(FilmBuffer<T>& fb,
                               const PinholeCamera& cam,
                               const Hitable& world,
                               const std::shared_ptr<const Hitable>& lights,
                               int nthreads = -1) -> void
{
  using namespace partou::math;

  int max_threads = std::thread::hardware_concurrency();
  nthreads = (nthreads < 1 || nthreads > max_threads) ? max_threads : nthreads;

  auto report_progress = [](std::ostream& os, const auto i, const auto n)
  {
    os << "\rtiling::parallel_tile_snap-> "
       << "tiles remaining: " << percent<int>(i, n) << "% (" << n - i << " out of " << n << ")"
       << std::flush;
  };

  const auto resolution = math::Vec2i {int(fb.nx()), int(fb.ny())};
  const auto tile_size = resolution / TILESIZE;
  const auto tile_count = tile_size.x * tile_size.y;

  std::atomic_int shared_tile_id = 0;
  auto thread_f = [&]
  {
    for (;;) {
      const int tile = shared_tile_id++;  // hopefully no race conditions here
      if (tile >= tile_count)
        return;

      auto&& offset = Vec2i {tile % tile_size.x, tile / tile_size.y} * TILESIZE;
      snap_tile(fb, cam, world, lights, resolution, std::move(offset));
    }
  };

  std::vector<std::future<void>> tasks(nthreads);
  for (int i = 0; i < nthreads; i++)
    tasks[i] = std::move(std::async(std::launch::async, thread_f));

  while (shared_tile_id < tile_count) {
    using namespace std::literals;

    report_progress(std::cout, shared_tile_id.load(), tile_count);
    std::this_thread::sleep_for(250ms);
  }

  report_progress(std::cout, tile_count, tile_count);

  for (int i = 0; i < nthreads; i++)  // shouldn't be needed, I put it here for good measure
    tasks[i].wait();
}
}  // namespace partou::tiling
