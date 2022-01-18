#pragma once

#include <iostream>

#include "../camera/pinhole_camera.hh"
#include "../color/color.hh"
#include "../film/film_buffer.hh"
#include "../material/material.hh"
#include "../math/general.hh"
#include "../random/random.hh"
#include "../sampling/pdf.hh"
#include "../shapes/hitable.hh"
//
#include "../shapes/mesh.hh"
//
#include "../perf_stats/stats.hh"

namespace partou::integrator::uniPathTracer
{
constexpr int_fast32_t TILESIZE = 16;
constexpr int TRACER_MAX_DEPTH = 50;
constexpr Spectrum background_color = sRGBSpectrum(0);

static auto traceRay(const partou::Ray& r,
                     const partou::Hitable& world,
                     std::shared_ptr<const Hitable>& lights,
                     int depth = TRACER_MAX_DEPTH) -> Spectrum
{  // Uni-directional path tracer
  using namespace partou;
  using namespace partou::math;

  constexpr math::Float eps = 1e-3;

  if (depth <= 0)  // exceeded the bounce limit, so no more light gathering.
    return Spectrum {0};

  hit_info hinfo;
  Float tBB;

  if (!(world.aabb().intersect(r, tBB)
        && world.hit(r, eps, std::numeric_limits<Float>::max(), hinfo)))  // If ray hits nothing
  {
    // color background: horizontal gradiant
    // auto unit_dir = r.dir().normalized();
    // auto t = (unit_dir.y + 1.0F) / 2.0F;
    // return math::interpolate_linear(math::Vec3f(.5, .7, 1), math::Vec3f(1), t);
    return background_color;
  }

  scatter_info sinfo;
  Spectrum emitted = hinfo.mat_ptr->emitted({}, hinfo, hinfo.p);
  if (!hinfo.mat_ptr->scatter(r, hinfo, sinfo))
    return emitted;

  if (sinfo.is_specular) {  // Handle specular surface separately, as it has no pdf.
    return sinfo.attenuation * traceRay(sinfo.specular_ray, world, lights, depth - 1);
  }

  const sampling::MixturePDF mixture_pdf {
      sinfo.pdf_ptr,
      std::make_shared<const sampling::HitablePDF>(lights, hinfo.p),
  };

  const auto r_scattered = Ray(hinfo.p, mixture_pdf.generate());
  const auto pdf_val = mixture_pdf.value(r_scattered.dir());

  auto albedo = sinfo.attenuation;
  albedo *= hinfo.mat_ptr->scattering_pdf(r, hinfo, r_scattered);
  albedo *= traceRay(r_scattered, world, lights, depth - 1) / pdf_val;
  return emitted + albedo;
}

static inline auto samplePixelJittered(const PinholeCamera& cam,
                                       const Hitable& world,
                                       std::shared_ptr<const Hitable>& lights,
                                       const math::Vec2i& pixel_coord,
                                       const math::Vec2i& resolution,
                                       const int& spp_sqrt) -> math::Vec3f
{  // Jittered supersampling
  using namespace partou::math;

  auto color = Vec3f {0};
  const auto substep = inv(Float(spp_sqrt));  // god I hope c++ gets UFCS one day
  const auto scaleInv = inv(Vec2f(resolution.x - 1, resolution.y - 1));

  for (int s_x = 0; s_x < spp_sqrt; s_x++) {
    for (int s_y = 0; s_y < spp_sqrt; s_y++) {
      const auto pixel_space_pos =
          Vec2f {pixel_coord[1] + s_x * substep + random::get(Float(0), substep),
                 pixel_coord[0] + s_y * substep + random::get(Float(0), substep)};

      const auto uv = pixel_space_pos * scaleInv;  // refactor into film.make_uv()

      auto r = cam.make_ray(uv[0], 1 - uv[1]);  // flip v because ppm-saver is upside down :(
      partou::stats::numPrimaryRays++;
      color += traceRay(r, world, lights);
      // std::cout << fmt::format("{}, {} = {}", j, i, color.x) << std::endl;
    }
  }
  return color;
}

template<typename T>
static inline auto serial_snap(FilmBuffer<T>& fb,
                               const PinholeCamera& cam,
                               const Hitable& world,
                               std::shared_ptr<const Hitable>& lights) -> void
{
  const auto resolution = math::Vec2i {int(fb.nx()), int(fb.ny())};
  for (int j = int(fb.ny()) - 1; j >= 0; j--) {
    if (j % 1 << 12 == 0)  // TODO: use progress bar
      std::cout << "\rtraceRay-> Scanlines remaining:\t\t" << j << ' ' << std::flush;

    for (int i = 0; i < int(fb.nx()); i++) {
      fb.pixel_color(j, i) =
          samplePixelJittered(cam, world, lights, {j, i}, resolution, fb.sample_per_pixel_sqrt);
    }
  }
}

}  // namespace partou::integrator::uniPath
