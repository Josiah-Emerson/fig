cmake_minimum_required(VERSION 3.22)
project(fig LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DLinux) # Change to -DWin32 to compile for windows or -DLinux for linux

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

file(GLOB_RECURSE UNFILTERED_SOURCE_FILES "src/*.cpp" "imgui/*.cpp")
set(EXCLUDE_DIR "${PROJECT_SOURCE_DIR}/Platform")
list(FILTER UNFILTERED_SOURCE_FILES EXCLUDE REGEX "${EXCLUDE_DIR}/.*")
set(src ${UNFILTERED_SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/imgui)


add_executable(fig ${src})
target_compile_options(fig PRIVATE -Wsign-conversion)

target_compile_features(fig PRIVATE cxx_std_17)
target_link_libraries(fig PUBLIC  
                               OpenGL::GL
                               X11::X11)
                            # OpenGL::GLU
# TESTS
# add_subdirectory(lib/Catch2)
# add_executable(fig-test test/Particle2D_test.cpp)
# target_link_libraries(fig-test PRIVATE Catch2::Catch2WithMain)

# cmake -DCMAKE_BUILD_TYPE=Debug
