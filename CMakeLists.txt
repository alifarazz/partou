cmake_minimum_required(VERSION 3.16)

project(
  partou
  VERSION 1.0.0
  LANGUAGES CXX)

set(PROJECT_IS_MAIN (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME))

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(PROJECT_IS_MAIN)
  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

include(cmake/get_cpm.cmake) # for dependencies

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
CPMAddPackage(
  NAME Ccache.cmake
  GITHUB_REPOSITORY TheLartians/Ccache.cmake
  VERSION 1.2.3
  )
endif(CCACHE_FOUND)

CPMAddPackage(
  NAME Imath
  GITHUB_REPOSITORY AcademySoftwareFoundation/Imath
  VERSION 3.1.4
)
if (Imath_ADDED)
  target_include_directories(Imath INTERFACE "$<BUILD_INTERFACE:${Imath_SOURCE_DIR}/src>")
endif()

CPMAddPackage(
  NAME pcg_cpp
  GITHUB_REPOSITORY imneme/pcg-cpp
  GIT_TAG ffd522e7188bef30a00c74dc7eb9de5faff90092
)
if (pcg_cpp_ADDED)
  set (use_pcg_definition USE_PCG)
  set (pcg_cpp_include_directories "${pcg_cpp_SOURCE_DIR}/include")
else()
  set (use_pcg_definition "")
  set (pcg_cpp_include_directories "")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${partou_SOURCE_DIR}/src/**/*.hh")
file(GLOB CC_FILES_LIST CONFIGURE_DEPENDS "${partou_SOURCE_DIR}/src/**/*.cc")
add_executable(${PROJECT_NAME} src/main.cc ${CC_FILES_LIST} ${HEADER_LIST})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE "${pcg_cpp_include_directories}")
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads Imath)
target_compile_definitions(${PROJECT_NAME} PRIVATE "${use_pcg_definition}")

add_custom_target(create-scenes-symlink ALL COMMAND
  ${CMAKE_COMMAND} -E create_symlink "${partou_SOURCE_DIR}/scenes" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/scenes"
)
