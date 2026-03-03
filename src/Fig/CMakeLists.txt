set(dir "Fig")
set(target "${project}_${dir}")
message(STATUS "${target} module")
add_library(${target} INTERFACE) # TODO: Remove interface when adding cpp files
add_library("${project}::${dir}" ALIAS ${target})

# SOURCES
# TODO: Add sources when we have cpp files for this dir 
# target_sources(${target} PUBLIC Fig.cpp)

# INCLUDE DIRECTORY
# TODO: see what happens when we don't target include dirs 
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# LINK TO OTHER MODULES (Maybe need this maybe don't?)
# target_link_lbraries(${target} PRIVATE )
