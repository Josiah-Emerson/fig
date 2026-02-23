#pragma once 
#include "../Platform/Platform.h" // TODO: modulize better so that this doesnt need to happen
// TODO: Nowhere near final 

class Renderer{
   private: 
      Platform& platform_;

   public: 
      Renderer(Platform& platform)
         : platform_ { platform }
      { }

      void render(const float R, const float G, const float B);
};
