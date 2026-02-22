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

      void run(){
         static bool open { true }; // TODO: Delete when done with demo window
         static int R { 0 };
         static int G { 0 };
         static int B { 0 };
         static int A { 0 };
         while(running_){
            Fig::EventQueue events = platform_->PollEvents();
            for(std::size_t i { 0 }; i < events.size(); ++i){
               ProcessEvent(events.at(i));
            }

            glClearColor(norm(&R), norm(&G), norm(&B), norm(&A)); 
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // TODO: would better way be to have Platform take in some type of of ImGui drawing function which is defined by Application 
            // run that and do its stuff ? 
            // For example, in here, one single call: platform_->DoImGui(fptr); where fptr is something (likely a func) which tells platform what it needs from imgui
            // and then in platform it does new frame, runs the function passed in to do the imgui stuff, and then renders the draw data?
            platform_->NewImGuiFrame();

            // New ImGui stuff: 

            constexpr int WIDTH { 200 };
            ImGui::SetNextWindowSize(ImVec2(WIDTH, platform_->GetWindowHeight()));
            ImGui::SetNextWindowPos(ImVec2(platform_->GetWindowWidth() - WIDTH, 0));
            if(ImGui::Begin("Hello, World", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
               ImGui::DragInt("R", &R, 1.0f, 0, 255); 
               ImGui::DragInt("G", &G, 1.0f, 0, 255); 
               ImGui::DragInt("B", &B, 1.0f, 0, 255); 
               ImGui::DragInt("A", &A, 1.0f, 0, 255); 
            }
            ImGui::End();

            // End of ImGui stuff

            platform_->RenderImGuiDrawData();
            platform_->SwapBuffers(); // TODO: Possibly bad practice to have the buffers swap in here? Might want to create an openGL class to use?
         }
      }

      void ProcessEvent(const Fig::Event& event){
         switch(event.Type){
            case(Fig::EventType::NULL_EVENT):
               std::cout << "NULL_EVENT\n";
               break;
            case(Fig::EventType::KEY_PRESS):
               std::cout << "KEY_PRESS\n";
               if(event.FKey.key == Fig::Key::ESCAPE){
                  running_ = false;
               }
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
            case(Fig::EventType::MOUSE_MOVEMENT):
               std::cout << "MOUSE_MOVEMENT\n";
               break;
            case(Fig::EventType::WINDOW_RESIZE):
               std::cout << "WINDOW_RESIZE\n";
               break;
            case(Fig::EventType::WINDOW_MOVE):
               std::cout << "WINDOW_MOVE\n";
               break;
            case(Fig::EventType::WINDOW_OTHER):
               std::cout << "WINDOW_OTHER\n";
               break;
            default: 
               std::cout << "HOWTF\n";
               break;
         }
      }

};
