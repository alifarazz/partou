import qbs

CppApplication {
    consoleApplication: true
    files:
        ["src/camera/camera.hh",
        "src/camera/simple_camera.cc",
        "src/camera/simple_camera.hh",
        "src/images/image.hh",
        "src/images/ppm.hh",
        "src/main.cc",
        "src/math/general.hh",
        "src/math/vec.hh",
        "src/random/random.hh",
        "src/ray/ray.hh",
        "src/shapes/hitable.hh",
        "src/shapes/hitable_list.cc",
        "src/shapes/hitable_list.hh",
        "src/shapes/sphere.cc",
        "src/shapes/sphere.hh",
        "src/shapes/triangle.cc",
        "src/shapes/triangle.hh",
        "src/utils/progress_bar.hh"]
    cpp.cxxLanguageVersion: "c++20"

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
