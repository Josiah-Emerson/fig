message(STATUS "Core")
set(target "Core")
add_Library(${target})

target_sources(${target} PRIVATE
   Application.cpp
   Layer.cpp
)

add_subdirectory(Window)

target_link_libraries(${target} PUBLIC 
   Core::Window
)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
