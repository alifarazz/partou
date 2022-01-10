#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
//
// #include <fmt/format.h>
//
#include "math/general.hh"
//
#include "camera/pinhole_camera.hh"
//
#include "io/obj.hh"
#include "io/ppm.hh"
//
#include "film/film_buffer.hh"
//
#include "color/color.hh"
//
#include "random/random.hh"
//
#include "shapes/hitable.hh"
#include "shapes/hitable_list.hh"
#include "shapes/mesh.hh"
#include "shapes/sphere.hh"
#include "shapes/triangle.hh"
//
#include "material/lambertian.hh"
//
#include "perf_stats/stats.hh"

using namespace partou;
using namespace partou::math;
using Time = std::chrono::high_resolution_clock;
using fsec = std::chrono::duration<float>;

////// globals
const auto aspect_ratio = 4.F / 3.F;  // 16.0F / 9.0F;
const int image_width = 640;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int spp_sqrt = 2;
const int tracer_max_depth = 30;

auto traceRay(const partou::Ray& r, const partou::Hitable& world, int depth = tracer_max_depth)
    -> Spectrum
{  // Uni-directional path tracer
  using namespace partou;

  if (depth <= 0)
    return Spectrum {0};

  constexpr math::Float eps = 1e-3;
  hit_info hinfo;
  math::Float tBB;
  if (world.aabb().intersect(r, tBB) && world.hit(r, eps, std::numeric_limits<Float>::max(), hinfo))
  {
    Ray r_scattered;
    Spectrum attenuation;
    if (hinfo.mat_ptr->scatter(r, hinfo, attenuation, r_scattered)) {
      return attenuation * traceRay(r_scattered, world, depth - 1);
    }
    return Spectrum {0};
  }

  // color background: horizontal gradiant
  auto unit_dir = r.dir().normalized();
  auto t = (unit_dir.y + 1.0F) / 2.0F;
  return interpolate_linear(Vec3f(.5, .7, 1.), Vec3f(1.0), t);
}

auto samplePixel(const PinholeCamera& cam, const Hitable& world, int x, int y) -> Vec3f
{  // Jittered supersampling
  auto color = Vec3f {0};
  const auto substep = 1 / Float(spp_sqrt);

  for (int s_x = 0; s_x < spp_sqrt; s_x++) {
    for (int s_y = 0; s_y < spp_sqrt; s_y++) {
      // Float u = static_cast<Float>(x) / (filmbuffer.nx() - 1);
      // Float v = static_cast<Float>(y) / (filmbuffer.ny() - 1);
      const auto pixel_space_pos = Vec2f {x + s_x * substep + random::get(Float(0), substep),
                                          y + s_y * substep + random::get(Float(0), substep)};

      const Vec2f uv = pixel_space_pos / Vec2f(image_width - 1, image_height - 1);

      auto r = cam.make_ray(uv[0], 1 - uv[1]);  // flip v because ppm-saver is upside down :(
      partou::stats::numPrimaryRays++;
      color += traceRay(r, world);
      // std::cerr << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
    }
  }
  return color;
}

auto main(int argc, char* argv[]) -> int
{
  using namespace partou;

  if (argc < 2) {
    std::cerr << "Bad args" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Film
  FilmBuffer<Vec3f> filmbuffer {image_height, image_width, spp_sqrt};

  // Camera
  const auto lookFrom = Point3f(0, 1, 4);  // Point3f(-2, 2, 1);
  // const auto lookFrom = Point3f(-2, 2, 1);
  const auto lookAt = Point3f(0, 0, -1);
  const auto vUp = Vec3f(0, 1, 0);
  const auto fov = Degree(25);
  const PinholeCamera cam(lookFrom, lookAt, vUp, fov, aspect_ratio);

  // Transform
  const auto lookLeft =
      spatial::Transform {Vec3f {.12, 1, 0}, Vec3f {.1}, Vec3f {0, PI / 6, PI / 6}};
  // const auto lookRight = spatial::Transform {Vec3f {0}, Vec3f {1}, Vec3f {0, -PI / 6, 0}};

  // Material
  const auto lambRed_p = std::make_shared<Lambertian>(sRGBSpectrum {1, 0, 0});
  const auto lambGreen_p = std::make_shared<Lambertian>(sRGBSpectrum {0, 1, 0});

  // Scene
  // const auto testSpheres = HitableList {
  //     {std::make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, nullptr),
  //      std::make_shared<Sphere>(Point3f(0, 0, -0.25f), .1, nullptr),

  //      std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, nullptr),
  //      std::make_shared<Sphere>(Point3f(-1.0, -0.5, -1.0), 0.5, nullptr),
  //      std::make_shared<Sphere>(Point3f(1.0, 0.5, -1.0), 0.5, nullptr),

  //      std::make_shared<Triangle>(
  //          Vec3f(-0.1, 0.1, -0.25), Vec3f(0.1, 0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),
  //      std::make_shared<Triangle>(
  //          Vec3f(-0.1, -0.1, -0.25), Vec3f(0.1, -0.1, -0.25), Vec3f(0, 0, -0.25 + .15), nullptr),

  //      std::make_shared<HitableList>(HitableList {{
  //          std::make_shared<Triangle>(
  //              Vec3f(-1, 1, -2), Vec3f(1, 1, -2), Vec3f(-1, -1, -2), nullptr),
  //          std::make_shared<Triangle>(
  //              Vec3f(1, 1, -2), Vec3f(1, -1, -2), Vec3f(-1, -1, -2), nullptr),
  //      }})}};

  // const auto cube = shape::Mesh {io::loader::OBJ("./cube.obj")};
  const auto suzanne =
      shape::Mesh {io::loader::OBJ("./suzanne.obj", true), lambRed_p}.apply(lookLeft);
  // const auto bunny1440 =
  //     shape::Mesh {io::loader::OBJ("stanford_bunny_1440.obj", true)}.apply(lookRight);
  // const auto bunny2880 = shape::Mesh{io::loader::OBJ("stanford_bunny_2880.obj")};
  // const auto dragon8710 = shape::Mesh {io::loader::OBJ("stanford_dragon_8710.obj")};

  // const auto monkey_bunny = HitableList {{
  //     std::make_shared<shape::Mesh>(suzanne),
  //     std::make_shared<shape::Mesh>(bunny1440),
  // }};

  const auto monkey_sphere = HitableList {{
      // std::make_shared<shape::Mesh>(suzanne),
      std::make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, lambRed_p),
      std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, lambGreen_p),
  }};

  const auto world = monkey_sphere;

  // Render
  const auto timeStart = Time::now();
  for (int j = int(filmbuffer.ny()) - 1; j >= 0; j--) {
    if (j % 1 << 12 == 0)  // TODO: use progress bar
      std::cerr << "\rtraceRay-> Scanlines remaining:\t\t" << j << ' ' << std::flush;

    for (int i = 0; i < int(filmbuffer.nx()); i++) {
      filmbuffer.pixel_color(j, i) = samplePixel(cam, world, i, j);
    }
  }
  const auto timeEnd = Time::now();
  std::cerr << std::endl;

  const auto saver = io::PPMImageSaver {filmbuffer};
  const auto outputfilename = argv[1];
  std::ofstream outputStream {outputfilename};
  saver.save(outputStream);

  //////
  //////
  fsec fs = timeEnd - timeStart;
  std::cout << std::string(80, '-') << std::endl;
  std::cout << "Render time                                 : " << fs.count() << " sec"
            << std::endl;
  // std::cout << "Total number of triangles                   : " << stats::numTriangles.load()
  //           << std::endl;
  std::cout << "Total number of primary rays                : " << stats::numPrimaryRays.load()
            << std::endl;
  std::cout << "Total number of ray-bbox tests              : " << stats::numRayBBoxTests.load()
            << std::endl;
  // std::cout << "Total number of ray-boundvolume tests       : "
  //           << stats::numRayBoundingVolumeTests.load() << qtd::endl;
  std::cout << "Total number of ray-triangles tests         : "
            << stats::numRayTrianglesTests.load() << std::endl;
  std::cout << "Total number of ray-triangles intersections : "
            << stats::numRayTrianglesIsect.load() << std::endl;
  std::cout << "Ray-triangles tests/intersects              : "
            << static_cast<float>(partou::stats::numRayTrianglesIsect.load())
                   / partou::stats::numRayTrianglesTests.load() * 100
            << "%" << std::endl;
}
