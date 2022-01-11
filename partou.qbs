import qbs

CppApplication {
    consoleApplication: true

    // generated with $ fd ".*\.(hh|cc)" src | xargs -I W echo \"W\",
    files: [
"src/accel/AABB.cc",
"src/accel/AABB.hh",
"src/camera/Camera.hh",
"src/camera/pinhole_camera.cc",
"src/camera/pinhole_camera.hh",
"src/color/color.hh",
"src/color/gamma2.hh",
"src/color/linear_gamma.hh",
"src/film/film_buffer.hh",
"src/integrator/uni_path_tracer.hh",
"src/io/obj.cc",
"src/io/obj.hh",
"src/io/ppm.hh",
"src/main.cc",
"src/material/lambertian.hh",
"src/material/material.hh",
"src/material/metal.hh",
"src/material/normal.hh",
"src/material/utils.hh",
"src/math/general.hh",
"src/math/mat.hh",
"src/math/transform.cc",
"src/math/transform.hh",
"src/math/vec.hh",
"src/perf_stats/stats.cc",
"src/perf_stats/stats.hh",
"src/random/random.hh",
"src/ray/ray.hh",
"src/shapes/hitable.hh",
"src/shapes/hitable_list.cc",
"src/shapes/hitable_list.hh",
"src/shapes/mesh.cc",
"src/shapes/mesh.hh",
"src/shapes/sphere.cc",
"src/shapes/sphere.hh",
"src/shapes/triangle.cc",
"src/shapes/triangle.hh",
"src/utils/progress_bar.hh",
]
    cpp.cxxLanguageVersion: "c++20"
    cpp.cxxFlags: ["-O3","-funroll-loops","-march=native","-mtune=native","-flto=4","-fgraphite-identity",
    "-floop-nest-optimize","-floop-parallelize-all","-DCHECK_SRGB_VALUES"]

    cpp.warningLevel: "all"

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
