#include "VectorTestLayer.h"
#include "Core_Utils/Linear/Vector.h"

bool VectorTestLayer::onEvent(Core::Events::Event& event){
   if(event.type == Core::Events::Type::KEY_PRESS){
      output();
      return true;
   }

   return false;
}



void VectorTestLayer::output(){
   using namespace Core::Linear;
   fvec3 vec1 { 30.4, 50.2, 92.43 };
   std::cout << "Magnitude of vec 1 " << vec1 << ": " << vec1.magnitude() << '\n';

   dvec3 vec2 { 40.21837, 12.3124, 13.354 };
   std::cout << "Magnitude of vec 2 " << vec2 << ": " << vec2.magnitude() << '\n';

   auto vec3 = vec2 - vec1;
   std::cout << "Vec3: " << vec3 << '\n';

   double dotab = dotProduct(vec2, vec3);
   double dotba = dotProduct(vec3, vec2);

   std::cout << "vec2 dot vec3: " << dotab << '\n';
   std::cout << "vec3 dot vec2: " << dotba << '\n';
}
