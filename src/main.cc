#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
//
// #include <fmt/format.h>
//
#include "math/general.hh"
//
#include "camera/PinholeCamera.hh"
//
#include "io/obj.hh"
//
#include "images/image.hh"
#include "images/ppm.hh"
//
#include "shapes/hitable.hh"
#include "shapes/hitable_list.hh"
#include "shapes/mesh.hh"
#include "shapes/sphere.hh"
#include "shapes/triangle.hh"

using namespace partou::math;

auto color_ray(const partou::Ray& r, const partou::Hitable& world) -> Vec3f
{
  using namespace partou;

  hit_info hinfo;
  if (world.hit(r, 0.0, std::numeric_limits<Float>::max(), hinfo)) {
    return (hinfo.normal + Vec3f(1.0F)) / 2.0F;
  }
  // color background: horizontal gradiant
  auto unit_dir = r.dir().normalized();
  auto t = (unit_dir.y + 1.0F) / 2.0F;
  return interpolate_linear(Vec3f(.5, .7, 1.), Vec3f(1.0), t);
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
  const auto lookFrom = Point3f(0, 0, 4);  // Point3f(-2, 2, 1);
  const auto lookAt = Point3f(0, 0, -1);  // Point3f(-2, 2, 1);
  const auto vUp = Vec3f(0, 1, 0);
  const auto fov = Degree(45);
  PinholeCamera cam(lookFrom, lookAt, vUp, fov, aspect_ratio);

  // Scene
  const auto world = HitableList {
      {std::make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, nullptr),
       std::make_shared<Sphere>(Point3f(0, 0, -0.25f), .1, nullptr),

       std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, nullptr),
       std::make_shared<Sphere>(Point3f(-1.0, -0.5, -1.0), 0.5, nullptr),
       std::make_shared<Sphere>(Point3f(1.0, 0.5, -1.0), 0.5, nullptr),

       std::make_shared<Triangle>(
           Vec3f(-0.1, 0.1, -0.25), Vec3f(0.1, 0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),
       std::make_shared<Triangle>(
           Vec3f(-0.1, -0.1, -0.25), Vec3f(0.1, -0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),

       std::make_shared<HitableList>(HitableList {{
           std::make_shared<Triangle>(
               Vec3f(-1, 1, -2), Vec3f(1, 1, -2), Vec3f(-1, -1, -2), nullptr),
           std::make_shared<Triangle>(
               Vec3f(1, 1, -2), Vec3f(1, -1, -2), Vec3f(-1, -1, -2), nullptr),
       }})}};
  const auto cube = shape::Mesh {io::OBJ_Loader("./cube.obj")};
  const auto suzanne = shape::Mesh {io::OBJ_Loader("./suzanne.obj")};

  // Render
  for (int j = int(filmbuffer.ny()) - 1; j >= 0; j--) {
    if (j & 0x00001000)  // TODO: use progress bar
      std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < int(filmbuffer.nx()); i++) {
      Float u = static_cast<Float>(i) / static_cast<Float>(filmbuffer.nx() - 1);
      Float v = static_cast<Float>(j) / static_cast<Float>(filmbuffer.ny() - 1);
      v = 1 - v;  // flip v because ppm saver is upside down :(

      auto r = cam.make_ray(u, v);
      // auto color = color_ray(r, world);
      auto color = color_ray(r, suzanne);
      // std::cerr << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
      filmbuffer.pixel_color(j, i) = color;
    }
  }

  // for (auto i : std::ranges::iota_view{2, 8})
  //   filmbuffer.pixel_color(i, 9) = Vec3f{1, 1, 1};

  auto saver = PPMImageSaver {filmbuffer};
  saver.save(std::cout);
}
