cmake_minimum_required(VERSION 3.22)
project(brownian LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

file(GLOB_RECURSE src "src/*.cpp" "imgui/*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/imgui)

add_executable(brownian ${src})
target_compile_options(brownian PRIVATE -Wsign-conversion)

target_compile_features(brownian PRIVATE cxx_std_17)
target_link_libraries(brownian PUBLIC  
                               OpenGL::GL
                               X11::X11)
                            # OpenGL::GLU
# TESTS
# add_subdirectory(lib/Catch2)
# add_executable(brownian-test test/Particle2D_test.cpp)
# target_link_libraries(brownian-test PRIVATE Catch2::Catch2WithMain)

# cmake -DCMAKE_BUILD_TYPE=Debug
