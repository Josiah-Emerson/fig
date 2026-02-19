#pragma once
#include "Platform.h"

class LinuxPlatform : public Platform {
   public: 
      //LinuxPlatform();
      FigEvent PollEvents() override {
         return FigEvent { };
      }
      void HelloWorld() const override;
};
