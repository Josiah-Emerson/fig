#pragma once 
#include "Platform/Platform.h"
#include <memory> // std::unique_ptr<>

class Application{
   private: 
      std::unique_ptr<Platform> platform_;

   public: 
      Application(std::unique_ptr<Platform> platform)
         : platform_ { std::move(platform) }
      {
         platform_->HelloWorld();
      }
};
