#pragma once
#include <Fig.h>

class IWindow {
   public: 
      virtual ~IWindow() = default;

   private: 
      Fig::Vec2i pos; // relative to parent 
      Fig::Vec2i size;
};
