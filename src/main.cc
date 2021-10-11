#include <iostream>
#include <limits>
#include <ranges>
#include <string>
//
// #include <fmt/format.h>
//
#include "math/general.hh"
//
#include "images/image.hh"
#include "images/ppm.hh"
//
#include "shapes/hitable.hh"
#include "shapes/hitable_list.hh"
#include "shapes/sphere.hh"

auto color_ray(const partou::Ray& r, partou::Hitable* world) -> partou::Vec3f
{
  using namespace partou;

  hit_info hinfo;
  if (world->hit(r, 0.0, std::numeric_limits<Float>::max(), hinfo)) {
    return (hinfo.normal + Vec3f(1.0f)) / 2.0f;
  }  // background
  auto unit_dir = r.dir().normalizedExc();
  auto t = (unit_dir.y + 1.0f) / 2.0f;
  return math::interpolate_linear(Vec3f(.5, .7, 1.), Vec3f(1.0), t);
}

auto main() -> int
{
  using namespace partou;

  constexpr int nx = 800, ny = 400;

  Vec3f lower_left_corner(-2., -1., -1.);
  Vec3f horizontal(4., 0., 0.);
  Vec3f vertical(0., 2., 0.);
  Vec3f origin(0., 0., 0.);

  auto sphere1 = Sphere(Vec3f(0, 0, -1), 0.8, nullptr);
  auto sphere2 = Sphere(Vec3f(0, -100.5, -1), 100.0, nullptr);
  Hitable* list[2] = {&sphere1, &sphere2};
  auto hitable_list = HitableList(list, 2);
  Hitable* world = &hitable_list;

  FilmBuffer<Vec3f> filmbuffer {ny, nx};

  for (int j = filmbuffer.ny() - 1; j >= 0; j--) {
    for (int i = 0; i < filmbuffer.nx(); i++) {
      Float u = static_cast<Float>(i) / static_cast<Float>(filmbuffer.nx());
      Float v = static_cast<Float>(j) / static_cast<Float>(filmbuffer.ny());

      auto r = Ray {origin, lower_left_corner + u * horizontal + v * vertical};

      // auto p = r.eval_at(2.0);  // why 2.0?
      auto color = color_ray(r, world);
      // std::cerr << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
      filmbuffer.pixel_color(j, i) = color;
    }
  }

  // for (auto i : std::ranges::iota_view{2, 8})
  //   filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  PPMImageSaver(filmbuffer).save(std::cout);
}
