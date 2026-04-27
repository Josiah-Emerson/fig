#pragma once
#include "Core_ECS/SortedComponentPool.h"
#include "Core_Utils/Concepts.h"
#include "Core_Utils/Types.h"
#include "SortedComponentPool.h"
#include <stdexcept>
#include <tuple>
#include <unordered_map>

// NOTE/TODO: calling addComponents/updateComponents (And maybe others) checks if an entity, 
// and then calls the individual add/update function, which also checks if that entity exists. This means 
// that a single addComponents call for 5 components on an entity checks if the ID exists 6 different times
// It also (although not SortedComponentPool is not implemented yet) probably checks the id in ComponentPool as well 

// TODO: How to actually iterate through
// For example: 
// posData = getContiguousComponentData<pos>();
// posData should be sorted based on Compare<U>, put how do we know where those splits are ?
// iterPairs: posPool.getIterators(); maybe pairs of beginning and end? 
// For shaders and model example (or something like that):
// struct U { Shader shader; Model model; };
// for(iter : iterPairs){
//    U u = getU(iter);
//    openGL.useProgram(shader);
//    openGL.bindVAO(model.VAO);
//    for(i {iter.first}; i < iter.second; ++i){
//       doSomething();
//    }
// }

namespace Core{

#define CLASS_TEMPLATE template<typename U, typename Compare, typename... Components> \
   requires((Concepts::is_component<Components> && ...) && \
   std::strict_weak_order<Compare, U, U>)
   
   CLASS_TEMPLATE
   class SortedRegistry{
      public: 
         SortedRegistry(std::size_t numEntities = 1000);

         SortedRegistry(const SortedRegistry& other) = default;
         SortedRegistry(SortedRegistry&& other) = default;
         SortedRegistry& operator=(const SortedRegistry& rhs) = default;
         SortedRegistry& operator=(SortedRegistry&& rhs) = default;
         ~SortedRegistry() = default;

         EntityID registerNewEntity(const U& comperand);

         // NOTE: Throws an exception if something goes wrong in adding components. 
         // TODO: Is this the behavior we want?
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                             (Concepts::all_types_unique<Args...>))
         EntityID registerNewEntity(const U& comperand, Args&&... args);

         bool containsEntity(const EntityID) const;

         void unregisterEntity(const EntityID);

         // TODO: Find a way to get rid of NOTE 1 (i.e. if one fails it figures out which ones succeeded and removes them)
         // Lowkey not hard, maybe just if it returns false, removeComponents?
         // Adds new components to an id
         // Returns true if and only if all components are successfully added 
         // NOTE 1: A return value of false does not mean that none of the components were added 
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                             (Concepts::all_types_unique<Args...>))
         bool addComponents(const EntityID id, Args&&... args);

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         bool addComponent(const EntityID id, T&& component);

         // NOTE: Same as addComponents, but a false return value does not mean none of the components were updated
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                             (Concepts::all_types_unique<Args...>))
         bool updateComponents(const EntityID id, Args&&... args);

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         bool updateComponent(const EntityID id, T&& component);

         // NOTE: returns after doing nothing if id not contained
         // NOTE: See note in implementation, but for now might throw an error if id is not found although it should never reach that point
         void updateComperand(const EntityID id, const U& comperand);

         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                             (Concepts::all_types_unique<Args...>))
         void removeComponents(const EntityID id);

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         void removeComponent(const EntityID id);

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         SortedComponentPool<T, Compare, U>& getPool();

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         const SortedComponentPool<T, Compare, U>& getPool() const;

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         T* getContiguousComponentData();

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         const T* getContiguousComponentData() const;

         // NOTE: If id is not in here it will throw an exception. 
         // See NOTE in updateComperand function for why 
         U& getComperand(const EntityID id);
         const U& getComperand(const EntityID id) const;

      private: 
         void internalReserve(std::size_t n);
         
      private: 
         std::tuple<SortedComponentPool<Components, Compare, U>...> m_pools;

         // TODO: This is (in theory but never know with STL) very efficient to get from EntityID -> U, however, 
         // getting going from U to all EntityIDs with that U will be quite slow. If we do this often may be good 
         // to implement a Bimap
         std::unordered_map<EntityID, U> m_entitiesToComperandMap; // Maps each entity ID to its comperand 
   };

   /*
    *
    * PUBLIC METHOD IMPLEMENTATIONS
    *
    */
   
   // NOTE: numEntities has a default value
   CLASS_TEMPLATE 
   SortedRegistry<U, Compare, Components...>::SortedRegistry(std::size_t numEntities)
      : m_pools { }
      , m_entitiesToComperandMap { }
   {
      internalReserve(numEntities);
      // TODO: Look into if reasoning is correct, (and if the init list above works as well), but 
      // here we reserve numEntities for each of the vectors for performance reasons, which allows no realloc 
      // later if numEntities guess is correct. Also, default initializing all of these elements in the vectors instead of reserving 
      // might zero out all the memory which could take time? Especially because we don't (or at least shouldn't) care about that 
   }

   CLASS_TEMPLATE
   EntityID SortedRegistry<U, Compare, Components...>::registerNewEntity(const U& comperand){
      static EntityID id { 0 };
      while(containsEntity(id))
         ++id;

      m_entitiesToComperandMap.insert(id, comperand);

      return id++;
   }

   CLASS_TEMPLATE
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) && 
                                       (Concepts::all_types_unique<Args...>))
   EntityID SortedRegistry<U, Compare, Components...>::registerNewEntity(const U& comperand, Args&&... args){
      EntityID id = registerNewEntity(comperand);

      if((!addComponent(id, std::forward<Args>(args)) && ...)){
         throw std::runtime_error("Error in registering new entity with components. One of the ComponentPools failed to add a component\n");
      }

      return id;
   }

   CLASS_TEMPLATE
   bool SortedRegistry<U, Compare, Components...>::containsEntity(const EntityID id) const{
      return m_entitiesToComperandMap.contains(id);
   }

   CLASS_TEMPLATE
   void SortedRegistry<U, Compare, Components...>::unregisterEntity(const EntityID id){
      removeComponents<Components...>(id);
      m_entitiesToComperandMap.erase(id);
   }

   CLASS_TEMPLATE
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                       (Concepts::all_types_unique<Args...>))
   bool SortedRegistry<U, Compare, Components...>::addComponents(const EntityID id, Args&&... args){
      if(!containsEntity(id))
         return false;

      return (!addComponent(id, std::forward<Args>(args)) && ...);
   }

   CLASS_TEMPLATE
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   bool SortedRegistry<U, Compare, Components...>::addComponent(const EntityID id, T&& component){
      if(!containsEntity(id))
         return false;

      SortedComponentPool<T, Compare, U>& pool = getPool<T, Compare, U>();
      return pool.insert(id, component, getComperand(id));
   }

   CLASS_TEMPLATE
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                       (Concepts::all_types_unique<Args...>))
   bool SortedRegistry<U, Compare, Components...>::updateComponents(const EntityID id, Args&&... args){
      if(!containsEntity(id))
         return false;

      return (updateComponent(id, std::forward<Args>(args)) && ...);
   }

   CLASS_TEMPLATE
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   bool SortedRegistry<U, Compare, Components...>::updateComponent(const EntityID id, T&& component){
      if(!containsEntity(id))
         return false;

      SortedComponentPool<T, Compare, U>& pool = getPool<T>();
      return pool.updateComponent(id, std::forward<T>(component));
   }

   CLASS_TEMPLATE
   void SortedRegistry<U, Compare, Components...>::updateComperand(const EntityID id, const U& comperand){
      if(!containsEntity(id))
         return;

      // NOTE: operator[] inserts id if not found which we don't want (but should not happen if implemented correctly)
      // for now we use at() which throws an exception if id is not found in the map
      
      m_entitiesToComperandMap.at(id) = comperand;
      std::apply(
            [id, &comperand](SortedComponentPool<Components, Compare, U>&... pools){
               (pools.updateComperand(id, comperand), ...);
            }, m_pools);
   }

   CLASS_TEMPLATE
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) && 
                              (Concepts::all_types_unique<Args...>))
   void SortedRegistry<U, Compare, Components...>::removeComponents(const EntityID id){
      (removeComponent<Args>(id) , ...);
   }

   CLASS_TEMPLATE
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   void SortedRegistry<U, Compare, Components...>::removeComponent(const EntityID id){
      SortedComponentPool<T, Compare, U>& pool = getPool<T>();
      pool.remove(id);
   }

   CLASS_TEMPLATE
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   SortedComponentPool<T, Compare, U>& SortedRegistry<U, Compare, Components...>::getPool(){
      return std::get<SortedComponentPool<T, Compare, U>>(m_pools);
   }

   CLASS_TEMPLATE 
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   const SortedComponentPool<T, Compare, U>& SortedRegistry<U, Compare, Components...>::getPool() const{
      return std::get<SortedComponentPool<T, Compare, U>>(m_pools);
   }

   CLASS_TEMPLATE 
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   T* SortedRegistry<U, Compare, Components...>::getContiguousComponentData() {
      SortedComponentPool<T, Compare, U>& pool = getPool<T, Compare, U>();
      return pool.contiguousData();
   }

   CLASS_TEMPLATE 
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   const T* SortedRegistry<U, Compare, Components...>::getContiguousComponentData() const{
      const SortedComponentPool<T, Compare, U>& pool = getPool<T, Compare, U>();
      return pool.contiguousData();
   }

   CLASS_TEMPLATE
   U& SortedRegistry<U, Compare, Components...>::getComperand(const EntityID id){
      return m_entitiesToComperandMap.at(id);
   }

   CLASS_TEMPLATE
   const U& SortedRegistry<U, Compare, Components...>::getComperand(const EntityID id) const{
      return m_entitiesToComperandMap.at(id);
   }

   /*
    *
    * PRIVATE MEMBER FUNCTION IMPLEMENTATIONS
    *
    */

   CLASS_TEMPLATE
   void SortedRegistry<U, Compare, Components...>::internalReserve(std::size_t n){
      std::apply(
            [n](SortedComponentPool<Components, Compare, U>&... pools){
               (pools.reserve(n), ...);
            }, m_pools);

      m_entitiesToComperandMap.reserve(n);
   }
   
#undef CLASS_TEMPLATE
} // namespace Core
