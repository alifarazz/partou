import qbs

CppApplication {
    consoleApplication: true

    // generated with $ fd ".*\.(hh|cc)" src | xargs -I W echo \"W\",
    files: [
"src/accel/AABB.cc",
"src/accel/AABB.hh",
"src/camera/Camera.hh",
"src/camera/PinholeCamera.cc",
"src/camera/PinholeCamera.hh",
"src/film/film_buffer.hh",
"src/io/obj.cc",
"src/io/obj.hh",
"src/io/ppm.hh",
"src/main.cc",
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
    cpp.cxxFlags: ["-Ofast","-funroll-loops","-march=native","-mtune=native","-flto"]

    cpp.warningLevel: "all"

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
