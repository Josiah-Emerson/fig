message(STATUS "Configuring executable ${project}")

# COMPILE FEATURES

add_executable(${project})



# TARGET SOURCES
target_sources(${project} PRIVATE
   OverlayLayer.cpp
   TestLayer.cpp
   UncaughtEventsOutputLayer.cpp
   main.cpp
)

# LINK MODULES FROM CORE
target_link_libraries(${project} PRIVATE 
   Core
   Core::ECS
   Core::Graphics
   Core::ImGui
   Core::Utils
)

# TODO: When include dirs
#target_include_directories(${project} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${project} PRIVATE -Wsign-conversion)
target_compile_features(${project} PUBLIC cxx_std_20)
