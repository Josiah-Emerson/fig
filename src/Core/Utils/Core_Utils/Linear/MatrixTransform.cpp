#include "MatrixTransform.h"
#include <cmath>

// NOTE: For now the only non-templated function is projection matrix, 
// and this might change because we might template it for the reasons listed in 
// TODO in MatrixTransform.h file, but for now, moving it into cpp file does 2 things:
// 1) allows to compile Core_Utils as non-interface without dummy.cpp (avoid annoying 'errors' within this module)
// 2) Leaving it in .h file would keep ODR warning

namespace Core{
   namespace Linear{
      Matrix<float, 4, 4> projectionMatrix(float AR, float FOV, float near, float far){
         // TODO: Understand what this actually does and why this works
         // TODO: simple optimizations like if it speeds up to construct in place? Or calculate near - far once ?
         FOV = degToRad(FOV);

         const float T = std::tan(FOV/2) * near;
         const float R = AR * T;

         Matrix<float, 4, 4> res;

         // Row 1
         res.at(0, 0) = near / R;
         res.at(0, 1) = 0;
         res.at(0, 2) = 0;
         res.at(0, 3) = 0;

         // row 2
         res.at(1, 0) = 0;
         res.at(1, 1) = near / T;
         res.at(1, 2) = 0;
         res.at(1, 3) = 0;

         // row 3
         res.at(2, 0) = 0;
         res.at(2, 1) = 0;
         res.at(2, 2) = ((far + near) / (near - far));
         res.at(2, 3) = ((2 * far * near) / (near - far));

         // row 4
         res.at(3, 0) = 0;
         res.at(3, 1) = 0;
         res.at(3, 2) = -1;
         res.at(3, 3) = 0;

         return res;
      }
   } // namespace Linear
} // namespace Core
