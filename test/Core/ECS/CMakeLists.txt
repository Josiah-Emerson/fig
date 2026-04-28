target_sources(${test-target} PRIVATE 
   SortedComponentPool.cpp
)

target_link_libraries(${test-target} PRIVATE 
   Core
   Core::ECS
)
