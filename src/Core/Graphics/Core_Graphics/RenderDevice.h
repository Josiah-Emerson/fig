#pragma once

namespace Core{
   // In a scene we are going to have objects
   // We will have a camera
   // We will use a render device implementation to draw things
   // Each scene should have a single render device, and the models should have a reference to that device?
   
   // Everything that can be drawn needs to have some kind of common drawable data. If we add an object to the scene it registers with the graphics device
   // If it is deleted it unregisters itself from the graphics device 
   // Each shader program may (will) have multiple drawable things attached to it, so it is probably smart to sort by shader ID and draw every item with that program?
   class RenderDevice{
      public: 
         RenderDevice() = default; // TBD: what goes in here
         virtual ~RenderDevice() = default;
         virtual void registerDrawable() = 0; // TBD: Args
         virtual void unregisterDrawable() = 0; // TBD: Args

         // registerDrawable()
         // unregisterDrawable()

      private: 
   };
   
} // namespace Core
