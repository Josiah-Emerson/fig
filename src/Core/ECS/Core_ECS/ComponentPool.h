#pragma once
#include "Core_Utils/Concepts.h"
#include "Core_Utils/Types.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// BIGGEST TODO (which might overrule below TODO): Probably should replace unordered_map with a bimap, or something similar (i.e. a one to one mapping)
// We know that for every entity id it can only refer to one index in m_pool, and vice versa. This greatly reduces our remove case which can be EXTREMELY slow
// TODO: Generally, we might need to look into the performance and tuning of unordered_map (or use of different type) for our specific cases
// See: https://stackoverflow.com/questions/28013801/c-unordered-map-good-number-of-buckets
// TODO: We use try catch blocks (as well as throwing std::out_of_range) at some points. 
// Need to determine how costly this is, and if we should do something different for handling errors 
// TODO: Add T&& val for insert and updates and stuff to allow for r-value stuff

namespace Core{
   template<typename T>
   requires(Concepts::is_component<T>)
   class ComponentPool{
      public: 
         ComponentPool();

         // Default number of entities
         // NOTE: This constructor is not too useful for Registry due to the constructor of std::tuple 
         // long story short, unless we figure out a different way, we need to default construct m_pools, 
         // which in turn default constructs ComponentPool
         ComponentPool(std::size_t numEntities);

         // Big 5
         ComponentPool(const ComponentPool& other) = delete;
         ComponentPool(ComponentPool&& other) = delete;
         ComponentPool& operator=(const ComponentPool& other) = delete;
         ComponentPool& operator=(ComponentPool&& other) = delete;
         ~ComponentPool() = default;

         // Reserves space for num elements
         // If num is less than current capacity, nothing happens
         void reserve(const std::size_t num);
         std::size_t size() const;
         bool contains(const EntityID id) const;

         // Inserts a new entity
         // returns true if the insert happened
         // false if otherwise. Reasons for returning false include: the ComponentPool already contains an entry for id
         bool insert(const EntityID id, const T& val);

         // Updates an existing entity
         // Returns true if the update happened
         // Returns false otherwise. Reasons for returning false include: ComponentPool does not contain id
         // Can theoretically throw std::out_of_range, however if it does there is a deeper issue with concurrency 
         // between our indices and entity IDs
         bool update(const EntityID id, const T& val);

         // NOTE: id(id) returns the component associated with id
         // Throws std:out_of_range exception if id does not exist
         T& id(const EntityID id);

         // NOTE: id(id) returns the component associated with id
         // Throws std:out_of_range exception if id does not exist
         const T& id(const EntityID id) const;

         // NOTE: operator[] returns the element at POSITION i, not associated with ID i
         // NOTE: undefined if i is not in range
         T& operator[](const std::size_t i);

         // NOTE: operator[] returns the element at POSITION i, not associated with ID i
         // NOTE: undefined if i is not in range
         const T& operator[](const std::size_t i) const;

         // Provides access to the underlying contiguous components
         // Iterating from this pointer will contain values up to, but NOT including contiguousData() + size()
         // NOTE: if ComponentPool is empty, returns a null pointer
         T* contiguousData();

         // Provides access to the underlying contiguous components
         // Iterating from this pointer will contain values up to, but NOT including contiguousData() + size()
         // NOTE: if ComponentPool is empty, returns a null pointer
         const T* contiguousData() const;

         // Removes the component of the entity specified by id
         // if entity is not in the pool, nothing happens
         // keeps underlying data contiguous
         void remove(const std::size_t id);

      private: 
         void internalReserve(const std::size_t n);

         // Throws std::out_of_range exception
         std::size_t getPoolIndex(const EntityID id) const;

         // EW SEE BIGGEST TODO
         // throws std::out_of_range if it doens't exist
         EntityID getIDFromIndex(const std::size_t poolIndex) const;


      private: 
         // NOTE: pool and poolMap are separated so that we can return a contiguous set of memory for this component
         std::vector<T> m_pool;
         std::unordered_map<EntityID, std::size_t> m_poolMap; // Is this even a good use of hash map?

   };

   /*
    *
    * PUBLIC METHOD IMPLEMENTATION
    *
    */
   template<typename T> requires(Concepts::is_component<T>)
   ComponentPool<T>::ComponentPool()
      : m_pool {}
      , m_poolMap {}
   { }

   template<typename T> requires(Concepts::is_component<T>)
   ComponentPool<T>::ComponentPool(std::size_t numEntities)
      : ComponentPool {} // TODO: if we need to do some basic setup for every ComponentPool in the future (also initializes m_pool and m_poolMap)
   {
      internalReserve(numEntities);
   }

   template<typename T> requires(Concepts::is_component<T>)
   void ComponentPool<T>::reserve(std::size_t num){
      if(num < m_pool.capacity())
         return;

      internalReserve(num);
   }

   template<typename T> requires(Concepts::is_component<T>)
   std::size_t ComponentPool<T>::size() const {
      return m_pool.size();
   }

   template<typename T> requires(Concepts::is_component<T>)
   bool ComponentPool<T>::contains(const EntityID id) const {
      return m_poolMap.contains(id);
   }

   template<typename T> requires(Concepts::is_component<T>)
   bool ComponentPool<T>::insert(const EntityID id, const T& val){
      if(m_poolMap.contains(id))
         return false;

      m_pool.push_back(val);
      const std::size_t INDEX = m_pool.size() - 1;
      // TODO: This uses insert(InputIt first, InputIt last) (i.e. overlad 7 on cppref) for some reason 
      // See if this is just my lsp not finding it correctly or actually what happens
      // m_poolMap.insert(id, INDEX);

      // NOTE: See above TODO, but we need to construct a value_type in order to 
      // insert as expected. value_type is defined by std::unordered_map as std::pair<Key, T>,
      // which in our case is std::pair<EntityID, std::size_t>. NOTE: T in this case references T for unordered_map not ComponentPool
      std::pair<EntityID, std::size_t> value { id, INDEX };
      auto res = m_poolMap.insert(value); 
      assert(res.second && "Error inserting into poolMap"); // TODO: For now just assert, but in the future either this doesn't really happen or if it does we remove from m_pool, and return false so that we can recover
      return true;
   }

   template<typename T> requires(Concepts::is_component<T>)
   bool ComponentPool<T>::update(const EntityID id, const T& val){
      if(!contains(id))
         return false;

      const std::size_t INDEX = getPoolIndex(id);
      m_pool[INDEX] = val;
      return true;
   }

   template<typename T> requires(Concepts::is_component<T>)
   T& ComponentPool<T>::id(const EntityID id){
      const std::size_t INDEX = getPoolIndex(id);
      return m_pool[INDEX];
   }

   template<typename T> requires(Concepts::is_component<T>)
   const T& ComponentPool<T>::id(const EntityID id) const{
      const std::size_t INDEX = getPoolIndex(id);
      return m_pool[INDEX];
   }

   template<typename T> requires(Concepts::is_component<T>)
   T& ComponentPool<T>::operator[](const std::size_t i){
      return m_pool[i];
   }

   template<typename T> requires(Concepts::is_component<T>)
   const T& ComponentPool<T>::operator[](const std::size_t i ) const {
      return m_pool[i];
   }

   template<typename T> requires(Concepts::is_component<T>)
   T* ComponentPool<T>::contiguousData(){
      if(m_pool.empty())
         return nullptr;

      return m_pool.data();
   }

   template<typename T> requires(Concepts::is_component<T>)
   const T* ComponentPool<T>::contiguousData() const{
      if(m_pool.empty())
         return nullptr;

      return m_pool.data();
   }

   template<typename T> requires(Concepts::is_component<T>)
   void ComponentPool<T>::remove(const std::size_t id){
      std::size_t index = 0;
      try{
         index = getPoolIndex(id);
      } catch(std::out_of_range&){
         // TODO: THIS SHOULD BE REDUNDANT IF THINGS ARE CORRECT, HOWEVER FOR NOW WE CAN USE IT TO VALIDATE
         // We should never get to a point where an id doesn't exist in m_pool but it does in m_poolMap
         assert(!contains(id) && "Somehow m_poolMap contains " + std::to_string(id) + ", but m_pool does not.\n");
         return;
      }

      update(id, m_pool.back());
      m_poolMap.erase(id);
 
      EntityID backID = getIDFromIndex(m_pool.size() - 1); // TODO: See biggest todo, but this func is likely very slow
      m_poolMap[backID] = index;
      m_pool.pop_back();

      // TODO: Remove asserts eventually
      assert(m_pool.size() == m_poolMap.size());
   }

   /*
    *
    * PRIVATE METHOD IMPLEMENTATIONS
    *
    */

   template<typename T> requires(Concepts::is_component<T>)
   void ComponentPool<T>::internalReserve(const std::size_t n){
      m_pool.reserve(n);
      m_poolMap.reserve(n);
   }

   template<typename T> requires(Concepts::is_component<T>)
   std::size_t ComponentPool<T>::getPoolIndex(const EntityID id) const{
      const auto iter = m_poolMap.find(id);
      if(iter == m_poolMap.end())
         throw std::out_of_range("Could not find pool index from ID. ID " + std::to_string(id) + " out of range.\n");

      return iter->second;
   }

   template<typename T> requires(Concepts::is_component<T>)
   EntityID ComponentPool<T>::getIDFromIndex(const std::size_t poolIndex) const{
      // NOTE: The following find_if assumes that andy index in m_pool belongs to one, and only one ID 
      const auto& iter = std::find_if(m_poolMap.begin(), m_poolMap.end(), 
            [poolIndex](const std::pair<const EntityID, std::size_t>& val){
               if(val.second == poolIndex)
                  return true;
               else
                  return false;
            });

      if(iter == m_poolMap.end()){
         throw std::out_of_range("in function ComponentPoop<T>::getIDFromIndex, the poolIndex was not found. Assuming this function is still only called in remove(), then a logical error occurred where the back index of m_pool does not exist in m_poolMap");
      }

      return iter->first;
   }

} // namespace Core
