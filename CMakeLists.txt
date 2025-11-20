cmake_minimum_required(VERSION 3.22)
project(brownian LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wsign-conversion)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_subdirectory(lib/matplotplusplus)

add_executable(brownian src/main.cpp 
                        src/Simulation.cpp)
target_compile_features(brownian PRIVATE cxx_std_17)
target_link_libraries(brownian 
                     PUBLIC matplot)

# TESTS
add_subdirectory(lib/Catch2)
add_executable(brownian-test test/Particle2D_test.cpp)
target_link_libraries(brownian-test PRIVATE Catch2::Catch2WithMain)
