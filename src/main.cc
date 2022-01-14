#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
//
#include "math/general.hh"
//
#include "camera/pinhole_camera.hh"
//
#include "io/obj.hh"
#include "io/ppm.hh"
//
#include "integrator/uni_path_tracer.hh"
//
#include "film/film_buffer.hh"
//
#include "shapes/hitable_list.hh"
#include "shapes/mesh.hh"
#include "shapes/sphere.hh"
#include "shapes/triangle.hh"
//
#include "material/dielectric.hh"
#include "material/emissive/diffuselight.hh"
#include "material/lambertian.hh"
#include "material/metal.hh"
//
#include "perf_stats/stats.hh"

using namespace partou;
using namespace partou::math;
using Time = std::chrono::high_resolution_clock;
using fsec = std::chrono::duration<float>;

////// globals
constexpr auto aspect_ratio = 1.f;  // 4.F / 3.F;  // 16.0F / 9.0F;
constexpr int image_width = 520;
constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
constexpr int spp_sqrt = 8;

int main(int argc, char* argv[])
{
  using namespace partou;
  using namespace partou::shape;

  if (argc < 2) {
    std::cerr << "Bad args" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Film
  FilmBuffer<Vec3f> filmbuffer {image_height, image_width, spp_sqrt};

  // Camera
  // const auto lookFrom = Point3f(0, 1, 4);  // Point3f(-2, 2, 1);
  // // const auto lookFrom = Point3f(-2, 2, 1);
  // const auto lookAt = Point3f(0, 0, -1);
  // const auto vUp = Vec3f(0, 1, 0);
  // const auto fov = Degree(45);

  // // conrell's
  const auto lookFrom = Point3f(0, 1, -4);  // Point3f(-2, 2, 1);
  // const auto lookFrom = Point3f(-2, 2, 1);
  const auto lookAt = Point3f(0, 1, 1);
  const auto vUp = Vec3f(0, 1, 0);
  const auto fov = Degree(36);  // 36

  const PinholeCamera cam(lookFrom, lookAt, vUp, fov, aspect_ratio);

  // Transform
  const auto lookLeft =
      spatial::Transform {Vec3f {.12, .1, 0}, Vec3f {1.1}, Vec3f {0, PI / 6, PI / 6}};
  // const auto lookRight = spatial::Transform {Vec3f {0}, Vec3f {1}, Vec3f {0, -PI / 6, 0}};

  // Material
  const auto lamb_red = std::make_shared<Lambertian>(sRGBSpectrum {1, 0, 0});
  const auto lamb_green = std::make_shared<Lambertian>(sRGBSpectrum {0, 1, 0});
  const auto lamb_ground = std::make_shared<Lambertian>(sRGBSpectrum {.8, .8, .0});
  const auto lamb_center_blue = std::make_shared<Lambertian>(sRGBSpectrum {.1, .2, .6});
  const auto lamb_center_egg = std::make_shared<Lambertian>(sRGBSpectrum {.7, .3, .3});
  const auto metal_left = std::make_shared<Metal>(sRGBSpectrum {.8, .8, .8}, .3);
  const auto metal_right = std::make_shared<Metal>(sRGBSpectrum {.8, .6, .2}, 1);
  const auto dielec_center = std::make_shared<Dielectric>(1.5);
  const auto dielec_left = std::make_shared<Dielectric>(1.5);

  const auto red = std::make_shared<Lambertian>(sRGBSpectrum(.65, .05, .05));
  const auto white = std::make_shared<Lambertian>(sRGBSpectrum(.73, .73, .73));
  const auto green = std::make_shared<Lambertian>(sRGBSpectrum(.12, .45, .15));
  const auto ceiling_light = std::make_shared<DiffuseLight>(Vec3f {5});
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

  auto backwall =
      Mesh {io::loader::OBJ("./scenes/objs/cornells box/backwall.obj")}.apply(spatial::Transform {
          Vec3f {0, 0, 0},
      });
  auto ceiling = Mesh {io::loader::OBJ("./scenes/objs/cornells box/ceiling.obj")};
  auto floor = Mesh {io::loader::OBJ("./scenes/objs/cornells box/floor.obj")};
  auto rightwall = Mesh {io::loader::OBJ("./scenes/objs/cornells box/rightwall.obj")};
  auto leftwall = Mesh {io::loader::OBJ("./scenes/objs/cornells box/leftwall.obj")};
  auto light = ceiling;
  light = light.apply(spatial::Transform {Vec3f {0, 1.39, 0}, Vec3f {.3}, Vec3f {0}});

  auto cube = Mesh {io::loader::OBJ("./cube.obj")};
  auto cube2 = cube;  // Mesh {io::loader::OBJ("./cube.obj")};
  cube = cube.apply(spatial::Transform(Vec3f {-.4, .25, -.6}, Vec3f {.25}, Vec3f {0, -PI / 18, 0}));
  cube2 =
      cube2.apply(spatial::Transform(Vec3f {.4, .5, -0}, Vec3f {.3, .5, .3}, Vec3f {0, PI / 18, 0}));
  // auto suzanne = Mesh {io::loader::OBJ("./suzanne.obj", true)}.apply(lookLeft);
  // const auto bunny1440 =
  //     Mesh {io::loader::OBJ("stanford_bunny_1440.obj", true)}.apply(lookRight);
  // const auto bunny2880 = Mesh{io::loader::OBJ("stanford_bunny_2880.obj")};
  // const auto dragon8710 = Mesh {io::loader::OBJ("stanford_dragon_8710.obj")};

  // const auto monkey_bunny = HitableList {{
  //     std::make_shared<Mesh>(suzanne),
  //     std::make_shared<Mesh>(bunny1440),
  // }};

  const auto metal_lamb_scene = HitableList {{
      std::make_shared<Sphere>(Point3f(0.0, -1000.5, -1.0), 1000.0, lamb_ground),
      std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, lamb_center_blue),
      // std::make_shared<Sphere>(Point3f(-1.0, 0.0, -1.0), 0.5, metal_left),
      // std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, dielec_center),
      std::make_shared<Sphere>(Point3f(-1.0, 0.0, -1.0), 0.5, dielec_left),
      std::make_shared<Sphere>(Point3f(1.0, 0.0, -1.0), 0.5, metal_right),
  }};

  cube.m_matptr = white;
  cube2.m_matptr = white;
  backwall.m_matptr = white;
  ceiling.m_matptr = white;
  floor.m_matptr = white;
  rightwall.m_matptr = red;
  leftwall.m_matptr = green;
  // light.m_matptr = lamb_center_blue;
  light.m_matptr = ceiling_light;
  const auto balls_with_light_scene = HitableList {{
      std::make_shared<Mesh>(backwall),
      std::make_shared<Mesh>(ceiling),
      std::make_shared<Mesh>(light),
      std::make_shared<Mesh>(floor),
      std::make_shared<Mesh>(rightwall),
      std::make_shared<Mesh>(leftwall),
      std::make_shared<Mesh>(cube),
      std::make_shared<Mesh>(cube2),
  }};

  // suzanne.m_matptr = dielec_left;
  // suzanne.m_matptr = metal_left;
  // suzanne.m_matptr = lamb_ground;
  const auto monkey_sphere = HitableList {{
      // std::make_shared<Mesh>(suzanne),
      std::make_shared<Mesh>(light),
      std::make_shared<Sphere>(Point3f(0.0, -100.5, -1.0), 100.0, lamb_red),
      std::make_shared<Sphere>(Point3f(0.0, 0.0, -1.0), 0.5, lamb_green),
  }};

  //   const auto world = metal_lamb_scene;
  // const auto world = monkey_sphere;
  const auto world = balls_with_light_scene;

  // Render
  const auto timeStart = Time::now();
  integrator::uniPath::snap(filmbuffer, cam, world);
  const auto timeEnd = Time::now();
  std::cerr << std::endl;

  const auto saver = io::PPMImageSaver {filmbuffer};
  const auto outputfilename = argv[1];
  std::ofstream outputStream {outputfilename};
  saver.save(outputStream);

  //////
  ////// Diagnostics
  fsec fs = timeEnd - timeStart;
  std::cout << std::string(80, '-') << std::endl;
  std::cout << "Render time                                 : " << fs.count() << " sec"
            << std::endl;
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
