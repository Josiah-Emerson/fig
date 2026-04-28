message("building tests")
set(test-target "test")

# find Catch2

add_executable(${test-target})
target_link_libraries(${test-target} PRIVATE 
   Catch2::Catch2WithMain
)

add_subdirectory(Core)
# add_subdirectory(Fig)
