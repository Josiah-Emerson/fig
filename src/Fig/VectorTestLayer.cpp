#include "VectorTestLayer.h"
#include "Core_Utils/Linear/Vector.h"
#include "Core_Utils/Linear/Matrix.h"

bool VectorTestLayer::onEvent(Core::Events::Event& event){
   if(event.type == Core::Events::Type::KEY_PRESS){
      output();
      return true;
   }

   return false;
}



void VectorTestLayer::output(){
   using namespace Core::Linear;
   fvec3 vec1 { 0.0, 1.1, 2.2 };
   fvec3 vec2 { 3.3, 4.4, 5.5 };
   fvec3 vec3 { 6.6, 7.7, 8.8 };

   Matrix<float, 3, 3> mat { vec1, vec2, vec3 };
   std::cout << mat;
}
