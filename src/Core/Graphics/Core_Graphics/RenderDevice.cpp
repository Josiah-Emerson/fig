#include "RenderDevice.h"
// TODO: Includes and extra supported graphics

#if defined(OPENGL)
#include "gl/GLRenderDevice.h"
#endif

namespace Core{
   bool GraphicsComperand::operator==(const GraphicsComperand& other) const{
      return (prog == other.prog) && (model == other.model);
   }

   bool GraphicsCompare::operator()(const GraphicsComperand& lhs, const GraphicsComperand& rhs) const{
      // NOTE: because prog and model are stored as shared_ptr, these sort by the ptr value (i think) 
      // This should be fine for now because I believe that it doesn't matter in what order they are grouped, 
      // Just that they are grouped

      if(lhs.prog < rhs.prog)
         return true;
      else if(lhs.prog > rhs.prog)
         return false;

      // progs are equal sort by inner now
      return lhs.model < rhs.model;
   }

   RenderDevice::RenderDevice(const PositionPool& positionPool)
      : m_positionPool { positionPool }
   { }

   std::shared_ptr<RenderDevice> RenderDevice::createRenderDevice(const PositionPool& positionPool){
      std::shared_ptr<RenderDevice> ret = nullptr;

      // TODO: Other graphics when ready
#if defined(OPENGL)
      ret = std::make_shared<GLRenderDevice>(positionPool);
#endif

      return ret;
   }
} // namespace Core
