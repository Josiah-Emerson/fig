set(dir "Utils")
set(target "Core_${dir}")
message(STATUS "${target} module")
add_library(${target} INTERFACE)
add_library("Core::${dir}" ALIAS ${target})

# SOURCES
target_sources(${target} PRIVATE
   # ${target}/Linear/Vector.cpp
)

# INCLUDE DIRECTORY
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# LINK TO LIBRARIES
target_link_libraries(${target} INTERFACE
   # Core
)
