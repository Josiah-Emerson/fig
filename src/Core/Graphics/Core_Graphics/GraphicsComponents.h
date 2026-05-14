#pragma once
#include "Core_Utils/Linear/Vector.h"
#include "Core_ECS/SortedRegistry.h"
#include <functional>

namespace Core{
   // GraphicsComperand and GraphicsCompare represent a way to sort a SortedRegistry and SortedComponentPool 
   // using relevant things like associated ShaderPrograms and Models to lessen switching in graphics layer (most likely contexts inside GPU)
   // For now it is sorted based on the ShaderProgram first and Model second, but can be changed later
   // This is what works with OpenGL (i think) best, and possibly can be changed if it is different across platforms


   // Define components here and add below 
   struct PositionTag {};
   struct DirectionTag {};
   struct ScaleTag {};
   struct ColorTag {};

   template<typename T, typename Tag>
   struct Component{
      using Type = T;
      T val;

      // references 
      operator T&() { return val; }
      operator const T&() const { return val; }

      // pointers 
      operator T*() { return &val; }
      operator const T*() const { return &val; }
   };

   using PositionComponent = Component<Linear::fvec3, PositionTag>;
   using DirectionComponent = Component<Linear::fvec3, DirectionTag>;
   using ScaleComponent = Component<Linear::fvec3, ScaleTag>;
   using ColorComponent = Component<Linear::fvec3, ColorTag>; // TODO: openGL takes colors from 0 to 1. 
                                                              // Might be a good idea to create an abstraction 
                                                              // here for our own color type, and allow each 
                                                              // specific implementation to do the conversion

   template<typename... Components>
   struct ComponentPack {};

   using GraphicsComponentList = ComponentPack<PositionComponent,
                                               DirectionComponent,
                                               ScaleComponent,
                                               ColorComponent>; // Add components to list here

   template<typename U, typename Compare, typename Pack>
   struct CreateGraphicsRegistry;

   template<typename U, typename Compare, typename... Components>
   struct CreateGraphicsRegistry<U, Compare, ComponentPack<Components...>>{
      using type = SortedRegistry<U, Compare, Components...>;
   };

   // function signature:
   template<typename Pack>
   struct CreateComponentFunction;

   template<typename... Components> 
   struct CreateComponentFunction<ComponentPack<Components...>>{
      using ArgType = std::tuple<const Components*...>;
      using type = std::function<void(void**, ArgType, std::size_t offset)>;
   };

   // End of gross type stuff

} // namespace Core
