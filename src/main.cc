#include <iostream>
#include <ranges>
#include <string>
//
#include "images/image.hh"
#include "images/ppm.hh"
//
#include "shapes/hitable.hh"
#include "shapes/hitable_list.hh"
#include "shapes/sphere.hh"

auto integrate(const partou::Ray& r, partou::Hitable* world) -> partou::Vec3f {}

auto main() -> int
{
  using namespace partou;

  constexpr int nx = 200, ny = 100;

  Vec3f lower_left_corner(-2., -1., -1.);
  Vec3f horizontal(4., 0., 0.);
  Vec3f vertical(0., 2., 0.);
  Vec3f origin(0., 0., 0.);

  auto sphere1 = Sphere(Vec3f(0, 0, -1), 0.5, nullptr);
  auto sphere2 = Sphere(Vec3f(0, -100.5, -1), 100, nullptr);
  Hitable* list[2] = {&sphere1, &sphere2};
  auto hitable_list = HitableList(list, 2);
  Hitable* world = &hitable_list;

  FilmBuffer<Vec3f> filmbuffer {ny, nx};

  for (int j = filmbuffer.ny(); j >= 0; j--) {
    for (int i = 0; i < filmbuffer.nx(); i++) {
      Float u = static_cast<Float>(i) / static_cast<Float>(filmbuffer.nx());
      Float v = static_cast<Float>(j) / static_cast<Float>(filmbuffer.ny());

      auto r = Ray {origin, lower_left_corner + u * horizontal + v * vertical};

      auto p = r.point_at_param(2.0);  // why 2.0?
      auto color = integrate(r, world);
      filmbuffer.pixel_color(j, i) = color;
    }
  }

  // for (auto i : std::ranges::iota_view{2, 8})
  //   filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  PPMImageSaver {filmbuffer, std::cout}.save();
}
