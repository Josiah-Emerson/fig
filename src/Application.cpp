#include "Application.h"
#include <iostream> // TODO: Delete when done

/*
run(){
   events = pollEvents();
   process(events);
   updateGui(); // defines what the gui should display based on application state (Imgui::Begin and ImGui::Render as render only finalizes draw data for frame)
   updatePhysics();
   render();

NEED: renderer, physicsEngine, Gui
}

render(){
   clearBackground();
   drawPhysics();
   drawImGuiData();
   swapBuffers();
}
*/

void Application::run(){
   static bool open { true }; // TODO: Delete when done with demo window
   static int R { 0 };
   static int G { 0 };
   static int B { 0 };
   static int A { 0 };

   while(running_){
      Fig::EventQueue events = platform_->PollEvents();
      for(std::size_t i { 0 }; i < events.size(); ++i){
         processEvent(events.at(i));
      }

      ui_->updateGui(&R, &G, &B);
      engine_->updatePhysics();
      renderer_->render(norm(&R), norm(&G), norm(&B));
   }
}

void Application::processEvent(const Fig::Event& event){
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
