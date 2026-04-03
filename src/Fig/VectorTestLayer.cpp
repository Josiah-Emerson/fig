#include "VectorTestLayer.h"
#include "Core_Utils/Linear/Vector.h"
#include "Core_Utils/Linear/Matrix.h"
#include <iomanip>

bool VectorTestLayer::onEvent(Core::Events::Event& event){
   if(event.type == Core::Events::Type::KEY_PRESS){
      output();
      return true;
   }

   return false;
}



void VectorTestLayer::output(){
   using namespace Core::Linear;
   // 4x3 3x3

   /* 
   std::cout << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10);
   dvec3 vec1 {3929.31038, 9804.72841, 5063.86373};
   dvec3 vec2 {6935.28254, 7699.60976, 3030.65272};
   dvec3 vec3 {6051.64732, 3844.55456, 6890.15868};
   dvec3 vec4 {1741.83093, 7555.56280, 3415.21740};

   dvec3 vec5 {9059.37915, 2793.78649, 3076.63488};
   dvec3 vec6 {3307.81531, 1392.47996, 6361.62214};
   dvec3 vec7 {5227.21071, 1810.54073, 7185.46577};
   // 6488.09236, 1666.50859,  588.44051

   Matrix<double, 4, 3> mat1 { vec1, vec2, vec3, vec4 };
   Matrix<double, 3, 3> mat2 { vec5, vec6, vec7 };
   std::cout << "mat1: " << mat1;
   std::cout << "mat2: " << mat2;

   auto res = mat1 * mat2;
   std::cout << "mat1 * mat2 = " << res;
   */
   // THIS EXAMPLE AS A FLOAT LOSES PRECISION
}
