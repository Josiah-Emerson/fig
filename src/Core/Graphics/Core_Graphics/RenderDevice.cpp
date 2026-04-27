#include "RenderDevice.h"
// TODO: Includes and extra supported graphics

#if defined(OPENGL)
#include "gl/GLRenderDevice.h"
#endif

namespace Core{
   std::shared_ptr<RenderDevice> RenderDevice::createRenderDevice(){
      std::shared_ptr<RenderDevice> ret = nullptr;

      // TODO: Other graphics when ready
#if defined(OPENGL)
      ret = std::make_shared<GLRenderDevice>();
#endif

      return ret;
   }
} // namespace Core
