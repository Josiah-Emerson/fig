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

   struct PositionComponent{using type = Linear::fvec3;}
   using PositionComponent = Linear::fvec3;
   using DirectionComponent = Linear::fvec3;
   using ScaleComponent = Linear::fvec3;
   using ColorComponent = Linear::fvec3;

   template<typename... Components>
   struct ComponentPack {};

   using GraphicsComponentList = ComponentPack<PositionComponent,
                                       DirectionComponent,
                                       ScaleComponent,
                                       ColorComponent>; // Add components to list here
   enum GraphicsComponentIndex : std::size_t { // NOTE: KEEP THIS ORDER THE SAME AS THE LIST ORDER
                                               // NOTE: If this order changes or an element is added, 
                                               // need to grep around and update areas of code which rely on 
                                               // this ordering. Very fragile
      POSITION = 0,
      DIRECTION,
      SCALE,
      COLOR,
      NUM_COMPONENTS
   };

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
      using ArgType = std::tuple<Components*...>;
      using type = std::function<void(void*, ArgType, std::size_t offset)>;
   };

   // End of gross type stuff

} // namespace Core
