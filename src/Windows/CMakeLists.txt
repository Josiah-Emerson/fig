set(dir "Windows")
set(target "${project}_${dir}")
message(STATUS "${target} module")
add_library(${target}) # TODO: remove interface when cpp files are added
add_library("${project}::${dir}" ALIAS ${target})

# SOURCES
# TODO: Add sources when we have cpp files for this dir 
target_sources(${target} PRIVATE 
   ${target}/RootWindow/LinuxRootWindow.cpp
   ${target}/RootWindow/OSXRootWindow.cpp
   ${target}/RootWindow/Win32RootWindow.cpp
)

# INCLUDE DIRECTORY
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# LINK TO OTHER MODULES
target_link_libraries(${target} PUBLIC 
   ${project}::Fig
)
