#include "Camera.h"
#include "Core_Utils/Linear/MatrixTransform.h"

namespace Core{

   Linear::fmat4 Camera::projectionMatrix(){
      return Linear::projectionMatrix(m_width / m_height, m_FOV, 0.1f, 100.f);
   }
} // namespace Core
