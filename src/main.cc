#include <iostream>
#include <limits>
#include <memory>
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

auto color_ray(const partou::Ray& r, partou::Hitable& world) -> partou::Vec3f
{
  using namespace partou;

  hit_info hinfo;
  if (world.hit(r, 0.0, std::numeric_limits<Float>::max(), hinfo)) {
    return (hinfo.normal + Vec3f(1.0f)) / 2.0f;
  }
  // color background: horizontal gradiant
  auto unit_dir = r.dir().normalized();
  auto t = (unit_dir.y + 1.0f) / 2.0f;
  return math::interpolate_linear(Vec3f(.5, .7, 1.), Vec3f(1.0), t);
}

auto main() -> int
{
  using namespace partou;

  // Film
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  FilmBuffer<Vec3f> filmbuffer {image_height, image_width};

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = Vec3f {0};
  auto horizontal = Vec3f(viewport_width, 0, 0);
  auto vertical = Vec3f(0, viewport_height, 0);
  auto lower_left_corner =
      origin - horizontal / 2 - vertical / 2 - Vec3f(0, 0, focal_length);

  // Scene
  auto world = HitableList {{
      std::make_shared<Sphere>(Vec3f(0, 0, -1), .1, nullptr),
      std::make_shared<Sphere>(Vec3f(0, -100.5, -1), 100.0, nullptr),
  }};

  // Render
  for (int j = int(filmbuffer.ny()) - 1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < int(filmbuffer.nx()); i++) {
      Float u = static_cast<Float>(i) / static_cast<Float>(filmbuffer.nx() - 1);
      Float v = static_cast<Float>(j) / static_cast<Float>(filmbuffer.ny() - 1);
      v = 1 - v; // flip v because ppm saver is upside down :(

      auto r = Ray {origin, lower_left_corner + u * horizontal + v * vertical};
      auto color = color_ray(r, world);
      // std::cerr << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
      filmbuffer.pixel_color(j, i) = color;
    }
  }

  // for (auto i : std::ranges::iota_view{2, 8})
  //   filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  PPMImageSaver(filmbuffer).save(std::cout);
}
