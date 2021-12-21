#include <iostream>
#include <limits>
#include <memory>
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
#include "shapes/triangle.hh"

auto color_ray(const partou::Ray& r, const partou::Hitable& world) -> partou::Vec3f
{
  using namespace partou;

  hit_info hinfo;
  if (world.hit(r, 0.0, std::numeric_limits<Float>::max(), hinfo)) {
    return (hinfo.normal + Vec3f(1.0F)) / 2.0F;
  }
  // color background: horizontal gradiant
  auto unit_dir = r.dir().normalized();
  auto t = (unit_dir.y + 1.0F) / 2.0F;
  return math::interpolate_linear(Vec3f(.5, .7, 1.), Vec3f(1.0), t);
}

auto main() -> int
{
  using namespace partou;

  // Film
  const auto aspect_ratio = 16.0F / 9.0F;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  FilmBuffer<Vec3f> filmbuffer {image_height, image_width};

  // Camera
  auto viewport_height = 2.0F;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0F;

  auto origin = Vec3f {0};
  auto horizontal = Vec3f(viewport_width, 0, 0);
  auto vertical = Vec3f(0, viewport_height, 0);
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3f(0, 0, focal_length);

  // Scene
  auto world = HitableList {{
      std::make_shared<Sphere>(Vec3f(0, 0, -0.25f), .1, nullptr),
      std::make_shared<Sphere>(Vec3f(0, -100.5, -1), 100.0, nullptr),
      std::make_shared<Triangle>(
          Vec3f(-0.1, 0.1, -0.25), Vec3f(0.1, 0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),
      std::make_shared<Triangle>(
          Vec3f(-0.1, -0.1, -0.25), Vec3f(0.1, -0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),
  }};

  // Render
  for (int j = int(filmbuffer.ny()) - 1; j >= 0; j--) {
    if (j & 0x00001000)  // TODO: use progress bar
      std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < int(filmbuffer.nx()); i++) {
      Float u = static_cast<Float>(i) / static_cast<Float>(filmbuffer.nx() - 1);
      Float v = static_cast<Float>(j) / static_cast<Float>(filmbuffer.ny() - 1);
      v = 1 - v;  // flip v because ppm saver is upside down :(

      auto r = Ray {origin, lower_left_corner + u * horizontal + v * vertical};
      auto color = color_ray(r, world);
      // std::cerr << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
      filmbuffer.pixel_color(j, i) = color;
    }
  }

  // for (auto i : std::ranges::iota_view{2, 8})
  //   filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  auto saver = PPMImageSaver {filmbuffer};
  saver.save(std::cout);
}
