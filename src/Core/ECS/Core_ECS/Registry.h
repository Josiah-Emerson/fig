#pragma once
#include "Core_Utils/Concepts.h"
#include <tuple>
#include <vector>

namespace Core{

   template<typename... Components>
   requires(Concepts::is_component<Components> && ...)
   class Registry{
      public:

         template<typename T>
         using ComponentPool = std::vector<T>;

         // numEntities should be a reasonable number of entities this registry can expect to track,
         // and allows the registry to not have to constantly grow 
         Registry(std::size_t numEntities = 1000);
         // TODO: BIG 5

         // register a new entity 
         // get a reference to a specific component of an entity 
         // remove an entity 
         // remove a specicic component of an entity 
         // check if an entity is a registered here
         // check what components an entity has here 
         // TODO: how do we handle if someone tries to access an entity that doesn't exist? 
         // TODO: how do we handle if someone wants a component of a valid entity but that entity doesn't have that component 

         // Returns a non-const reference to all 
         template<typename T>
         requires(std::is_same_v<T, Components> || ...)
         ComponentPool<T>& getPool();

         template<typename T>
         requires(std::is_same_v<T, Components> || ...)
         const ComponentPool<T>& getPool() const;

      private: 
         
         std::tuple<ComponentPool<Components>...> m_pools;
         IdType m_numEntities { 0 };
   };

   /*
    *
    * PUBLIC METHOD IMPLEMENTATIONS
    *
    */
   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   Registry<Components...>::Registry(std::size_t numEntities)
      : m_pools {}  // TODO: Does this default construct the std::vectors themselves? Read up on std:::tuple constructors
   {
      // TODO: Look into if reasoning is correct, (and if the init list above works as well), but 
      // here we reserve numEntities for each of the vectors for performance reasons, which allows no realloc 
      // later if numEntities guess is correct. Also, default initializing all of these elements in the vectors instead of reserving 
      // might zero out all the memory which could take time? Especially because we don't (or at least shouldn't) care about that 
      constexpr std::size_t NUM_COMPONENTS = std::tuple_size_v<std::tuple<Components...>>;
      // TODO: If we ever in the future need to keep a list of what types m_pools contains, 
      // then perhaps update this for loop (and others) to use that list to explicitly do something 
      // like ComponentPool<Component>& pool = std::get<i>(m_pools); instead of auto& 
      for(std::size_t i { 0 }; i < NUM_COMPONENTS; ++i){
         auto& pool = std::get<i>(m_pools);
         pool.reserve(numEntities);
      }
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(std::is_same_v<T, Components> || ...)
   Registry<Components...>::ComponentPool<T>& Registry<Components...>::getPool(){
      return std::get<T>(m_pools);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(std::is_same_v<T, Components> || ...)
   const Registry<Components...>::ComponentPool<T>& Registry<Components...>::getPool() const{
      return std::get<T>(m_pools);
   }
} // namespace Core
