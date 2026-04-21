#include "VectorTestLayer.h"
#include <iostream>
#include <random>

bool VectorTestLayer::onEvent(Core::Events::Event& event){
   if(event.type == Core::Events::Type::KEY_PRESS){
      output();
      return true;
   }

   return false;
}

VectorTestLayer::VectorTestLayer()
   : m_registry { }
{
   constexpr int NUM_ENTITIES { 1000 };
   std::cout << "In Vector Test Layer constructor, after Registry construction\n";
   std::cout << "Inserting " << NUM_ENTITIES << " entities into registry\n";
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dist(-10000, 10000);

   Core::EntityID id;
   for(int i { 0 }; i < NUM_ENTITIES; ++i ){
      id = m_registry.registerEntity();
      m_registry.addComponent<PositionComponent>(id, {{dist(gen), dist(gen), dist(gen)}} );
   }

   std::cout << "Done adding elements\n";
}


std::ostream& operator<<(std::ostream& out, const PositionComponent& pos){
   out << pos.pos;
   return out;
}
template<typename T>
void print(const Core::ComponentPool<T>& pool){
   for(std::size_t i { 0 }; i < pool.size(); ++i){
      std::cout << "Element at position " << i << ": ";
      std::cout << pool[i] << '\n';
   }
}

void VectorTestLayer::output(){
   print(m_registry.getPool<PositionComponent>());
   std::cout << "Mult each by 2\n";
   Core::ComponentPool<PositionComponent>& pool = m_registry.getPool<PositionComponent>();
   for(std::size_t i { 0 }; i < pool.size(); ++i){
      pool[i].pos = pool[i].pos * 2;
   }
   print(m_registry.getPool<PositionComponent>());
}
