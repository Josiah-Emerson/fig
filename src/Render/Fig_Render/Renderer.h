#pragma once 
#include "Fig_Platform/Platform.h"
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
