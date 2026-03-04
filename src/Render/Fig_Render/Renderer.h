#pragma once 
#include "Fig_Windows/IWindow.h"
// TODO: Nowhere near final 

class Renderer{
   private: 
      PlatformRootWindow& platform_;

   public: 
      Renderer(PlatformRootWindow& platform)
         : platform_ { platform }
      { }

      void render(const float R, const float G, const float B);
};
