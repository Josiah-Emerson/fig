#pragma once
#include "Core_Utils/Concepts.h"
#include "Core_Utils/Types.h"
#include "ComponentPool.h"
#include <limits>
#include <tuple>
#include <unordered_set>

// TODO: If we are constantly checking whether a pool contains an ID or not, we might be best to have a bitset for that?

namespace Core{
   template<typename... Components>
   requires(Concepts::is_component<Components> && ...)
   class Registry{
      public:
         // numEntities should be a reasonable number of entities this registry can expect to track,
         // and allows the registry to not have to constantly grow 
         Registry(std::size_t numEntities = 1000);

         Registry(const Registry& other) = default;
         Registry(Registry&& other) = default;
         Registry& operator=(const Registry& rhs) = default;
         Registry& operator=(Registry&& rhs) = default;
         ~Registry() = default;

         // register a new entity 
         EntityID registerEntity();

         // check if an entity is a registered here
         bool isRegistered(const EntityID id);

         // remove an entity 
         void unregisterEntity(const EntityID id);

         // TODO: Find a way to get rid of NOTE 1 (i.e. if one fails it figures out which ones succeeded and removes them)
         // Lowkey not hard, maybe just if it returns false, removeComponents?

         // Adds new components to an id
         // Returns true if and only if all components are successfully added 
         // NOTE 1: A return value of false does not mean that none of the components were added 
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ... ) &&
                                             (Concepts::all_types_unique<Args...>))
         bool addComponents(const EntityID id, Args&&... args);

         // Adds a new component to an ID
         // returns true if and only if the component is successfully added 
         // returns false if unsuccessful (inaccurate id, already added component, etc...)
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         bool addComponent(const EntityID id, T&& component);

         // Update components of existing id
         // NOTE 1: This is not the only way of updating a component. Could get non const reference to the pool
         // NOTE 2: An underlying function from ComponentPool can throw an exception, however it will only propegate
         // to this point if our logic is incorrect
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) && 
                                             (Concepts::all_types_unique<Args...>))
         bool updateComponents(const EntityID, Args&&... args);

         // Update component of existing id
         // TODO/NOTE 0: For the future should we insert into ComponentPool if it doesn't contain it? For now no
         // NOTE 1: This is not the only way of updating a component. Could get non const reference to the pool
         // NOTE 2: An underlying function from ComponentPool can throw an exception, however it will only propegate
         // to this point if our logic is incorrect
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         bool updateComponent(const EntityID id, T&& component);

         // Check if an entity has a component of type T attached to it
         // NOTE: returns false if id doesn't exist in registry
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         bool hasComponent(const EntityID id) const;

         // get a reference to a specific component of an entity 
         // throws std::out_of_range if id doesn't have component T
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         T& getComponent(const EntityID id);

         // get a reference to a specific component of an entity 
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         const T& getComponent(const EntityID id) const;


         // TODO: This is complex due to working with dynamic types, and may not even be useful
         // auto getAllComponents(const EntityID id);
         // Something like: std::tuple<C...> allComponents(EntityID), where type C is all the components EntityID has associated with it
         // const auto getAllComponents(const EntityID id) const;


         // removes list of components from the EntityID
         template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                             (Concepts::all_types_unique<Args...>))
         void removeComponents(const EntityID id);

         // remove a specific component of an entity 
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         void removeComponent(const EntityID id);
         
         
         // TODO: how do we handle if someone tries to access an entity that doesn't exist? 
         // TODO: how do we handle if someone wants a component of a valid entity but that entity doesn't have that component 

         // Returns a non-const reference to all 
         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         ComponentPool<T>& getPool();

         template<typename T> requires(Concepts::is_in_pack<T, Components...>)
         const ComponentPool<T>& getPool() const;

      private: 
         void internalReserve(const std::size_t n);

      private: 
         std::tuple<ComponentPool<Components>...> m_pools;
         std::unordered_set<EntityID> m_registeredEntities; // TODO: Would a bitset or std::vector<bool> be better?
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
      internalReserve(numEntities);

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
   EntityID Registry<Components...>::registerEntity(){
      // TODO: Should hopefully not happen, but might be a good check? Might also be a good idea to remove for perfomance builds
      assert(m_registeredEntities.size() < std::numeric_limits<EntityID>::max() && "Max number of entites reached");
      static EntityID ID;
      while(m_registeredEntities.contains(ID))
         ++ID;

      m_registeredEntities.insert(ID);
      return ID;
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   bool Registry<Components...>::isRegistered(const EntityID id){
      return m_registeredEntities.contains(id);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   void Registry<Components...>::unregisterEntity(const EntityID id){
      std::apply(
            [id](ComponentPool<Components>&... pool){
               (pool.remove(id), ...);
            }, m_pools);

      m_registeredEntities.erase(id);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                        (Concepts::all_types_unique<Args...>))
   bool Registry<Components...>::addComponents(const EntityID id, Args&&... args){
      /*  See TODO in class declaration, but this is a lambda fold which could be helpful
       template<typename... Args>
bool addComponents(EntityID id, Args&&... args) {
    return ([&]() {
        if (!addComponent(id, std::forward<Args>(args))) {
            // You could log the failure here if you wanted
            return false;
        }
        return true;
    }() && ...);
       */
      return (addComponent(id, std::forward<Args>(args)) && ...);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   bool Registry<Components...>::addComponent(const EntityID id, T&& component){
      if(!isRegistered(id)) 
         return false;

      ComponentPool<T>& pool = getPool<T>();
      return pool.insert(id, std::forward<T>(component));
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) && 
                                      (Concepts::all_types_unique<Args...>))
   bool Registry<Components...>::updateComponents(const EntityID id, Args&&... args){
      return (updateComponent(id, std::forward<Args>(args)) && ...);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   bool Registry<Components...>::updateComponent(const EntityID id, T&& component){
      if(!isRegistered(id))
         return false;

      ComponentPool<T>& pool = getPool<T>();
      return pool.update(id, std::forward<T>(component));
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   bool Registry<Components...>::hasComponent(const EntityID id) const{
      const ComponentPool<T>& pool = getPool<T>();
      return pool.contains(id);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   T& Registry<Components...>::getComponent(const EntityID id){
      ComponentPool<T>& pool = getPool<T>();
      return pool.id(id);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   const T& Registry<Components...>::getComponent(const EntityID id) const{
      const ComponentPool<T>& pool = getPool<T>();
      return pool.id(id);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename... Args> requires((Concepts::is_in_pack<Args, Components...> && ...) &&
                                       (Concepts::all_types_unique<Args...>))
   void Registry<Components...>::removeComponents(const EntityID id){
      (removeComponent<Args>(id), ...);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   void Registry<Components...>::removeComponent(const EntityID id){
      ComponentPool<T>& pool = getPool<T>();
      pool.remove(id);
   }



   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   ComponentPool<T>& Registry<Components...>::getPool(){
      return std::get<T>(m_pools);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   template<typename T> requires(Concepts::is_in_pack<T, Components...>)
   const ComponentPool<T>& Registry<Components...>::getPool() const{
      return std::get<T>(m_pools);
   }

   template<typename... Components> requires(Concepts::is_component<Components> && ...)
   void Registry<Components...>::internalReserve(const std::size_t n){
      std::apply(
            [n](ComponentPool<Components>&... pool){
               (pool.reserve(n), ...);
            }, m_pools);

      m_registeredEntities.reserve(n);
   }
} // namespace Core
