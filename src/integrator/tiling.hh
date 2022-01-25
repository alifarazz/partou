#pragma once

#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
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

  std::cout << "\rserial_tile_snap\ttile size: " << TILESIZE << 'x' << TILESIZE << '\n';

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

  std::cout << "\rparallel_tile_snap\tsqrt spp: " << fb.sample_per_pixel_sqrt << '\n';
  std::cout << "\rparallel_tile_snap\ttile size: " << TILESIZE << 'x' << TILESIZE << '\n';

  constexpr int deltas_size = 1 << 3;  // can further be optimzed, no need to hold all the values
  constexpr std::chrono::milliseconds sleep_duration(350);
  auto report_progress =
      [=, deltas = std::array<int, deltas_size> {0}, prev_i = 0, delay = deltas_size](
          std::ostream& os, const int i, const int n) mutable
  {
    for (int i = 0; i < deltas_size - 1; i++)
      deltas[i] = deltas[i + 1];
    deltas[deltas_size - 1] = i - prev_i;
    prev_i = i;

    constexpr std::chrono::milliseconds one_sec(1000);
    constexpr Float ratio = Float(one_sec.count()) / Float(sleep_duration.count());

    const Float avg_render_speed_per_sleep_duration =
        std::accumulate(deltas.begin(), deltas.end(), 0) / Float(deltas.size());
    const Float speed = ratio * avg_render_speed_per_sleep_duration;
    const int secs = std::round((n - i) / speed);

    os << "\rparallel_tile_snap\t"
       << "tiles remaining: " << percent<int>(i, n) << "% (" << n - i << " out of " << n << ")";
    if (delay) {
      std::cout << std::flush;
      delay--;
      return;
    } else {
      std::cout << ", time remaining: " << secs << " secs      " << std::flush;
    }
  };

  const auto resolution = math::Vec2i {int(fb.ny()), int(fb.nx())};
  const auto tile_size = resolution / TILESIZE;
  const auto tile_count = tile_size.x * tile_size.y;

  std::atomic_int shared_tile_id = 0;
  auto thread_f = [&]
  {
    for (;;) {
      const int tile = shared_tile_id++;  // hopefully no race conditions here
      if (tile >= tile_count)
        return;

      auto offset = Vec2i {tile / tile_size[1], tile % tile_size[1]} * TILESIZE;
      snap_tile(fb, cam, world, lights, resolution, offset);
    }
  };

  int max_threads = std::thread::hardware_concurrency();
  nthreads = (nthreads < 1 || nthreads > max_threads) ? max_threads : nthreads;
  std::cout << "\rparallel_tile_snap\trender threads count: " << nthreads << '\n';

  std::vector<std::future<void>> tasks(nthreads);
  for (int i = 0; i < nthreads; i++)
    tasks[i] = std::move(std::async(std::launch::async, thread_f));

  while (shared_tile_id < tile_count) {
    using namespace std::literals;

    report_progress(std::cout, shared_tile_id.load(), tile_count);
    std::this_thread::sleep_for(sleep_duration);
  }

  report_progress(std::cout, tile_count, tile_count);

  // for (int i = 0; i < nthreads; i++)  // shouldn't be needed, I put it here for good measure
  //   tasks[i].wait();
}
}  // namespace partou::tiling
