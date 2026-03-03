message(STATUS "Configuring executable ${project}")

add_executable(${project})

find_package(OpenGL REQUIRED)
target_link_libraries(${project} PRIVATE OpenGL::GL) # OpenGL::GLU may be needed in future

if(FIG_TARGET_OS_LINUX) # TODO: Extend for future Win32 and OSX
   find_package(X11 REQUIRED)
   target_link_libraries(${project} PRIVATE X11::X11)
endif()

# SUBDIRECTORIES
message(STATUS "Adding subdirectories")
# Future optional/different subdirectory branching logic would go here
add_subdirectory(Engine)
add_subdirectory(Fig)
add_subdirectory(Platform)
add_subdirectory(Render)
add_subdirectory(Windows)

# Target sources
target_sources(${project} PRIVATE 
   ${imgui_files}
   Application.cpp
   Ui.cpp
   main.cpp
)

target_link_libraries(${project} PRIVATE 
   ${project}::Engine
   ${project}::Platform
   ${project}::Windows
   ${project}::Render
)

target_include_directories(${project} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# COMPILE FEATURES
target_compile_options(${project} PRIVATE -Wsign-conversion)
target_compile_features(${project} PRIVATE cxx_std_17)

# TESTS
# add_subdirectory(lib/Catch2)
# add_executable(fig-test test/Particle2D_test.cpp)
# target_link_libraries(fig-test PRIVATE Catch2::Catch2WithMain)

