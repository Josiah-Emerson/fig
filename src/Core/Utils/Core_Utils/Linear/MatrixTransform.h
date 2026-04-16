#pragma once
#include "Core_Utils/Linear/Matrix.h"
#include "Core_Utils/Concepts.h"
#include "Core_Utils/Linear/Vector.h"
#include <cmath>
#include <cstddef>

// Helpful functions for different transformations using Matrices. Mostly creating matrices which can be used to transform a vector
// TODO: in modelMAtrix need to implement direction facing as well as if scale is anything but 0 does it work correctly?

namespace Core{
   namespace Linear{
      /*
       *
       * DECLARATIONS
       *
       */

      // returns square identity matrix of size M
      template<typename T, std::size_t M>
      requires(Concepts::numeric<T>)
      Matrix<T, M, M> getIdentityMatrix();

      // TODO: Implement direction vector and test any other scale than 1. Note when implenting d, need to uncomment V2 at all spots
      // Creates a Matrix which can transform a vector in model coordinates to world coordinates
      // p is the center of the model in world coordinates
      // d is the direction the model is facing in world coordinates 
      // s represents how much to scale the model by
      template<typename V1, /*typename V2,*/ typename V3, typename Mat = Concepts::float_promotion_t<V1, /*V2,*/ V3>>
      requires(Concepts::numeric<V1, /*V2,*/ V3>)
      Matrix<Mat, 4, 4> modelMatrix(const Vector<V1, 3>& p, /*const Vector<V2, 3>& d,*/ const Vector<V3, 3>& s);

      // Creates a matrix which transforms a vector in world coordinates to a coordinate system where the +z axis 
      // is the unit vector from point lookAt to point pos (i.e. the correct direction for openGL if you wanted to look at point lookAt),
      // and the x and y axes are defined by the right hand rule, where y is the thumb, and in the direction of up
      template<typename V1, typename V2, typename V3, typename Mat = Concepts::float_promotion_t<V1, V2, V3>>
      requires(Concepts::numeric<V1, V2, V3>)
      Matrix<Mat, 4, 4> viewMatrix(const Vector<V1, 3>& pos, const Vector<V2, 3>& lookAt, const Vector<V3, 3>& up);

      // TODO: perhaps implement other functions for producing projection matrices from different values. For example this function calculates essentially 6 things from 4 input values
      // Could have one function with those 6 input values and other functions which take different inputs and funnel them into this 'common' one
      // perhaps also have some type of way to switch projection methods (i.e. orthogonal) could do some fun stuff with this
      // TODO: For now must return float, but I want to look into a way to return float by default or a different floating point value if specified
      // Constructs a projection matrix for a camera with aspect ratio AR, vertical fov FOV in degrees, and a near and far clipping distance
      Matrix<float, 4, 4> projectionMatrix(float AR, float FOV, float near, float far);

      // TODO: when a reason for another math-like file happens, should maybe move this to that
      template<typename T, typename U = Concepts::float_promotion_t<T>> requires(Concepts::numeric<T>)
      U degToRad(T val);

      /*
       *
       * IMPLEMENTATIONS
       *
       */

      template<typename T, std::size_t M>
      requires(Concepts::numeric<T>)
      Matrix<T, M, M> getIdentityMatrix(){
         Matrix<T, M, M> res(0);
         for(std::size_t i { 0 }; i < M; ++i){
            res.at(i, i) = 1;
         }

         return res;
      }

      // TODO: See note below. check if possible and if so add something to ensure this doesn't happen
      // NOTE: Mat is a default type. One issue might be if a user defines Mat as something like int.
      template<typename V1, /*typename V2,*/ typename V3, typename Mat>
      requires(Concepts::numeric<V1, /*V2,*/ V3>)
      Matrix<Mat, 4, 4> modelMatrix(const Vector<V1, 3>& p, /*const Vector<V2, 3>& d,*/ const Vector<V3, 3>& s){
         /*
            Worked out on paper, where posVec has world positions (x, y,z ), dirVec (soon to be rotation TBD), and scaleVec has scale:
            M = T * R * S, where T is translation matrix, S is rotation matrix, and S is scale matrix
            A = R * S -> For now, no rotation, so R is I which means A is S
            M = T * A  -> M = T * S (see above)
            worked out: 
            M = 
               | Sx 0  0  Tx |
               | 0  Sy 0  Ty |
               | 0  0  Sz Tz |
               | 0  0  0  1  | 
          */
         auto res = getIdentityMatrix<Mat, 4>();

         // Row 1
         res.at(0, 0) = s[0];
         res.at(0, 3) = p[0];

         // Row 2
         res.at(1, 1) = s[1];
         res.at(1, 3) = p[1];
         
         // Row 3
         res.at(2, 2) = s[2];
         res.at(2, 3) = p[2];

         return res;
         
         // TODO: if this throws a bunch of warnings, we can possibly wrap each row in a static cast?
         // UPDATED TODO: I think it would be more efficient (maybe) to do this, but casting makes things confusing
         /*
         return { static_cast<Vector<Mat, 4>>( {s[0], 0, 0, p[0] ), 
                static_cast<Vector<Mat, 4>>( 0, s[1], 0, p[1] ), 
                static_cast<Vector<Mat, 4>>( 0, 0, s[2], p[2] ), 
                static_cast<Vector<Mat, 4>>( 0, 0, 0, 1 ) };
                */
      }


      // NOTE: Mat is a default arg
      template<typename V1, typename V2, typename V3, typename Mat>
      requires(Concepts::numeric<V1, V2, V3>)
      Matrix<Mat, 4, 4> viewMatrix(const Vector<V1, 3>& pos, const Vector<V2, 3>& lookAt, const Vector<V3, 3>& up){
         // TODO: Perhaps may need to static cast if warnings get annoying
         // TODO: Would it be more efficient to instead of creating R and T matrices and multiplying, to figure it out by hand and do the products? Most likely

         // Creates a matrix which transforms a vector in world coordinates to a coordinate system where the +z axis 
         // is the unit vector from point lookAt to point pos (i.e. the correct direction for openGL if you wanted to look at point lookAt),
         // the x and y axes are defined by the right hand rule, where y is the thumb, and in the direction of up, and it is centered around position pos

         // khat
         Vector<Mat, 3> khat { pos - lookAt };
         khat.normalize();

         // ihat
         Vector<Mat, 3> ihat { crossProduct(up, khat) };
         ihat.normalize();

         // jhat
         Vector<Mat, 3> jhat { crossProduct(khat, ihat ) };

         auto R = getIdentityMatrix<Mat, 4>();
         auto T = getIdentityMatrix<Mat, 4>();
         
         // TODO: Would be nice to do something like R.row(1) = ihat
         // TOOD: Or would be nice to do Matrix<> R {ihat, jhat, khat, 1.f} and it creates the proper matrix

         // TODO: Figure out why we are placing ihat, jhat and khat into rows and not the columns? 
         // Row 1 (ihat)
         R.at(0,0) = ihat[0];
         R.at(0,1) = ihat[1];
         R.at(0,2) = ihat[2];

         // Row 2 (jhat)
         R.at(1, 0) = jhat[0];
         R.at(1, 1) = jhat[1];
         R.at(1, 2) = jhat[2];

         // Row 3 (khat)
         R.at(2, 0) = khat[0];
         R.at(2, 1) = khat[1];
         R.at(2, 2) = khat[2];

         // Translation mat
         // -pos because we are moving the object not the camera? idk
         T.at(0, 3) = -pos[0];
         T.at(1, 3) = -pos[1];
         T.at(2, 3) = -pos[2];

         return R * T;
      }

      // U is default arg
      template<typename T, typename U> requires(Concepts::numeric<T>)
      U degToRad(T val){
         return val * (M_PI / 180);
      }

   } // namespace Linear
} // namespace Core
