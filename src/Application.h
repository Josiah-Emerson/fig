#pragma once 
#include "Platform/Platform.h"
#include "Engine/Engine.h"
#include "Render/Renderer.h"
#include "Ui.h"
#include <memory> // std::unique_ptr<>

class Application{
   private: 
      std::unique_ptr<Platform> platform_;
      std::unique_ptr<Engine> engine_;
      std::unique_ptr<Renderer> renderer_;
      std::unique_ptr<Ui> ui_;
      bool running_ { true };

   public: 
      Application(std::unique_ptr<Platform> platform)
      : platform_ { std::move(platform) }
      , engine_ { std::make_unique<Engine>() }
      , renderer_ {std::make_unique<Renderer>(*platform_.get()) }
      , ui_ { std::make_unique<Ui>(*platform_.get()) }
      {
      }
      // TODO: Big 5? TBD

      // TODO: very temp as it clamps to 0-255 and then normalizes this range to 1
      void clamp(int* x){
         if(*x < 0){
            *x = 0;
         }else if (*x > 255){
            *x = 255;
         }
      }

      float norm(int* x){
         clamp(x);
         return (*x) / 255.f;
      }

      void run();
      void processEvent(const Fig::Event& event);
};
