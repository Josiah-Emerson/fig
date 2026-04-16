#pragma once
#include "Core_Utils/Linear/Vector.h"
#include "Core_Utils/Linear/Matrix.h"
#include "Core_Utils/Linear/MatrixTransform.h"

// TODO: Should look be a direction or a point?
// TODO: We could store proj and view matrices as members and only update them when something which depends on them is updated 
// For example only update view matrix when pos, look or up changes. This would allow us to not calc these every frame, and only 
// when an update is needed. We can return a ptr to these things to items which need it?

namespace Core{
   class Camera{
      public:
         // Constructors
         Camera() = delete;
         Camera(const Linear::fvec3& pos, float FOV, float height, float width, const Linear::fvec3& look)
            : m_pos { pos }
            , m_look { look }
            , m_FOV { FOV }
            , m_height { height }
            , m_width { width }
         { }

         // Big 5
         Camera(const Camera&) = delete;
         Camera(Camera&&) = delete;
         Camera& operator=(const Camera&) = delete;
         Camera& operator=(Camera&&) = delete;
         ~Camera() = default;

         // MVP = P * V * M
         template<typename T = float>
         auto viewMatrix();

         Linear::fmat4 projectionMatrix();

      private:
         Linear::fvec3 m_pos; // Position of camera in world coordinates
         Linear::fvec3 m_look; // Where the camera is looking in world coordinates
         float m_FOV; // vertical FOV in degrees
         float m_height;
         float m_width;
   };

   // NOTE:: T is a default arg
   template<typename T>
   auto Camera::viewMatrix(){
      return Linear::viewMatrix(m_pos, m_look, Linear::Vector(0, 1, 0));
   }

} // namespace Core
