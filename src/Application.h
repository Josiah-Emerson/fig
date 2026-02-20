#pragma once 
#include "Platform/Platform.h"
#include <imgui.h>
#include <GL/gl.h>
#include <memory> // std::unique_ptr<>
#include <iostream> // TODO: Delete when done

class Application{
   private: 
      std::unique_ptr<Platform> platform_;
      bool running_ { true };

   public: 
      Application(std::unique_ptr<Platform> platform)
         : platform_ { std::move(platform) }
      {
      }
      // TODO: Big 5? TBD

      void run(){
         static bool open { true }; // TODO: Delete when done with demo window
         while(running_){
            Fig::EventQueue events = platform_->PollEvents();
            for(std::size_t i { 0 }; i < events.size(); ++i){
               ProcessEvent(events.at(i));
            }

            platform_->NewImGuiFrame();
            if(open)
               ImGui::ShowDemoWindow(&open);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            platform_->RenderImguiDrawData();
            platform_->SwapBuffers();
         }
      }

      void ProcessEvent(const Fig::Event& event){
         switch(event.Type){
            case(Fig::EventType::NULL_EVENT):
               std::cout << "NULL_EVENT\n";
               break;
            case(Fig::EventType::KEY_PRESS):
               std::cout << "KEY_PRESS\n";
               break;
            case(Fig::EventType::KEY_RELEASE):
               std::cout << "KEY_RELEASE\n";
               break;
            case(Fig::EventType::BUTTON_PRESS):
               std::cout << "BUTTON_PRESS\n";
               break;
            case(Fig::EventType::BUTTON_RELEASE):
               std::cout << "BUTTON_RELEASE\n";
               break;
            case(Fig::EventType::WINDOW_RESIZE):
               std::cout << "WINDOW_RESIZE\n";
               break;
            default: 
               std::cout << "HOWTF\n";
               break;
         }
      }

};
